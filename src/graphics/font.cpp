#include <fstream>
#include <sstream>
#include <cmath>
#include <cstring>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

#include "exlib/graphics/image.hpp"
#include "exlib/graphics/font.hpp"

namespace ex {

inline static unsigned long read(FT_Stream rec, unsigned long offset, unsigned char* buffer, unsigned long count) {
    auto* stream = (std::istream*) (rec->descriptor.pointer);

    stream->clear();
    stream->seekg((std::streamoff) offset, std::ios::beg);
    if (!stream->good())
        return 0;

    if (count > 0) {
        stream->read((char*) (buffer), (std::streamsize) count);
        return (unsigned long) (stream->gcount());
    }

    return 0;
}

inline static void close(FT_Stream) {}

inline static std::streamsize get_stream_size(std::istream& stream) {
    auto old_pos = stream.tellg();
    stream.seekg(0, std::ios::end);
    auto size = stream.tellg();
    stream.seekg(old_pos);

    return size;
}

template <typename T, typename U>
inline static T reinterpret(const U& input) {
    T output;
    std::memcpy(&output, &input, sizeof(U));
    return output;
}

inline static unsigned long long combine(float outline_thickness, bool bold, unsigned int index) {
    return ((unsigned long long) (reinterpret<unsigned int>(outline_thickness)) << 32)
         | ((unsigned long long) (bold) << 31) 
         | index;
}

struct Font::FontHandles
{
    FontHandles() = default;

    ~FontHandles()
    {
        FT_Stroker_Done(stroker);
        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }

    FontHandles(const FontHandles&) = delete;
    FontHandles& operator=(const FontHandles&) = delete;
    FontHandles(FontHandles&&) = delete;
    FontHandles& operator=(FontHandles&&) = delete;

	FT_Library   library = {};
	FT_StreamRec stream_rec = {};
	FT_Face      face = {};
	FT_Stroker   stroker = {};
};

Font::Font(const std::filesystem::path& path) {
	if (!open_from_file(path))
        EX_THROW("Failed to open font from file '" + path.string() + "'");
}

Font::Font(const void* data, int size) {
	if (!open_from_memory(data, size))
		EX_THROW("Failed to open font from memory");
}

Font::Font(const Font& other) 
	: font_handles(other.font_handles),  info(other.info), pages(other.pages), pixel_buffer(other.pixel_buffer) {}

Font Font::copy() const {
	return *this;
}

bool Font::open_from_file(const std::filesystem::path& path) {
    using namespace std::string_view_literals;

    cleanup();

    auto stream = std::make_shared<std::ifstream>();
    stream->open(path, std::ios::binary);

    if (!stream->is_open()) {
        EX_ERROR("Failed to load font (failed to open file '" + path.string() + "')");
        return false;
    }

    if (open_from_stream_impl(*stream, "file")) {
        stream_ = stream;
        return true;
    }

    return false;
}

bool Font::open_from_memory(const void* data, int size) {
    cleanup();

    if (!data) {
        EX_ERROR("Failed to load image from memory (null data pointer)");
        return false;
    }

    auto stream = std::make_shared<std::stringstream>();
    stream->write((const char*) (data), size);
    stream->seekg(0, std::ios::beg);

    if (open_from_stream_impl(*stream, "memory")) {
        stream_ = stream;
        return true;
    }

    return false;
}

const Font::Info& Font::get_info() const {
	return info;
}

bool Font::has_glyph(char32_t code_point) const {
    return FT_Get_Char_Index(font_handles ? font_handles->face : nullptr, code_point);
}

const Glyph& Font::get_glyph(char32_t code_point, unsigned int char_size, bool bold, float outline_thickness) const {
    GlyphTable& glyphs = load_page(char_size).glyphs;

    unsigned long long key = combine(
        outline_thickness,
        bold,
        FT_Get_Char_Index(font_handles ? font_handles->face : nullptr, code_point)
    );

    if (const auto it = glyphs.find(key); it != glyphs.end()) {
        return it->second;
    }

    const Glyph glyph = load_glyph(code_point, char_size, bold, outline_thickness);
    return glyphs.try_emplace(key, glyph).first->second;
}

float Font::get_kerning(unsigned int first, unsigned int second, unsigned int char_size, bool bold) const {
    if (first == 0 || second == 0)
        return 0.0f;

    FT_Face face = font_handles ? font_handles->face : nullptr;

    if (face && set_current_size(char_size)) {
        FT_UInt index1 = FT_Get_Char_Index(face, first);
        FT_UInt index2 = FT_Get_Char_Index(face, second);

        float first_rsb_delta = (float) (get_glyph(first, char_size, bold).rsb_delta);
        float second_lsb_delta = (float) (get_glyph(second, char_size, bold).lsb_delta);

        FT_Vector kerning = { 0, 0 };
        if (FT_HAS_KERNING(face))
            FT_Get_Kerning(face, index1, index2, FT_KERNING_UNFITTED, &kerning);

        if (!FT_IS_SCALABLE(face))
            return (float) (kerning.x);

        return std::floor(
            (second_lsb_delta - first_rsb_delta + (float) (kerning.x) + 32) / float(1 << 6)
        );
    }

    return 0.0f;
}

float Font::get_line_spacing(unsigned int char_size) const {
    FT_Face face = font_handles ? font_handles->face : nullptr;

    if (face && set_current_size(char_size)) {
        return (float) (face->size->metrics.height) / float(1 << 6);
    }

    return 0.0f;
}

float Font::get_underline_position(unsigned int char_size) const {
    FT_Face face = font_handles ? font_handles->face : nullptr;

    if (face && set_current_size(char_size)) {
        if (!FT_IS_SCALABLE(face))
            return (float) (char_size) / 10.0f;

        return -(float) (FT_MulFix(face->underline_position, face->size->metrics.y_scale)) / float(1 << 6);
    }

    return 0.0f;
}

float Font::get_underline_thickness(unsigned int char_size) const {
    FT_Face face = font_handles ? font_handles->face : nullptr;

    if (face && set_current_size(char_size)) {
        if (!FT_IS_SCALABLE(face))
            return (float) (char_size) / 14.0f;

        return (float) (FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) / float(1 << 6);
    }

    return 0.0f;
}

const Texture& Font::get_texture(unsigned int char_size) const {
	return load_page(char_size).texture;
}

void Font::cleanup() {
    font_handles.reset();

    pages.clear();
    std::vector<unsigned char>().swap(pixel_buffer);

    stream_.reset();
}

bool Font::open_from_stream_impl(std::istream& stream, std::string type) {
    cleanup();

    auto handles = std::make_shared<FontHandles>();

    if (FT_Init_FreeType(&handles->library)) {
        EX_ERROR("Failed to load font from " + type + " (failed to init FreeType)");
        return false;
    }

    handles->stream_rec.base = nullptr;
    handles->stream_rec.size = (unsigned long) (get_stream_size(stream));
    handles->stream_rec.pos = 0;
    handles->stream_rec.descriptor.pointer = &stream;
    handles->stream_rec.read = &read;
    handles->stream_rec.close = &close;

    FT_Open_Args args;
    args.flags = FT_OPEN_STREAM;
    args.stream = &handles->stream_rec;
    args.driver = nullptr;

    FT_Face face = nullptr;
    if (FT_Open_Face(handles->library, &args, 0, &face)) {
        EX_ERROR("Failed to load font from " + type + " (failed to create the font face)");
        return false;
    }
    handles->face = face;

    if (FT_Stroker_New(handles->library, &handles->stroker)) {
        EX_ERROR("Failed to load font from " + type + " (failed to create the stroker)");
        return false;
    }

    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        EX_ERROR("Failed to load font from " + type + " (failed to set the Unicode character set)");
        return false;
    }

    font_handles = std::move(handles);
    info.family = face->family_name ? face->family_name : std::string();

    return true;
}

Font::Page& Font::load_page(unsigned int char_size) const {
	return pages.try_emplace(char_size).first->second;
}

Glyph Font::load_glyph(char32_t code_point, unsigned int char_size, bool bold, float outline_thickness) const {
    Glyph glyph;

    if (!font_handles)
        return glyph;

    FT_Face face = font_handles->face;
    if (!face)
        return glyph;

    if (!set_current_size(char_size))
        return glyph;

    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (outline_thickness)
        flags |= FT_LOAD_NO_BITMAP;
    if (FT_Load_Char(face, code_point, flags))
        return glyph;

    FT_Glyph glyph_desc = nullptr;
    if (FT_Get_Glyph(face->glyph, &glyph_desc))
        return glyph;

    FT_Pos weight = 1 << 6;
    bool outline = (glyph_desc->format == FT_GLYPH_FORMAT_OUTLINE);
    if (outline) {
        if (bold) {
            FT_OutlineGlyph outline_glyph = (FT_OutlineGlyph) (glyph_desc);
            FT_Outline_Embolden(&outline_glyph->outline, weight);
        }

        if (outline_thickness) {
            FT_Stroker stroker = font_handles->stroker;

            FT_Stroker_Set(stroker,
                           (FT_Fixed) (outline_thickness * float(1 << 6)),
                           FT_STROKER_LINECAP_ROUND,
                           FT_STROKER_LINEJOIN_ROUND,
                           0);
            FT_Glyph_Stroke(&glyph_desc, stroker, true);
        }
    }

    FT_Glyph_To_Bitmap(&glyph_desc, FT_RENDER_MODE_NORMAL, nullptr, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) (glyph_desc);
    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    if (!outline) {
        if (bold)
            FT_Bitmap_Embolden(font_handles->library, &bitmap, weight, weight);

        if (outline_thickness)
            EX_ERROR("Failed to outline glyph (no fallback available)");
    }

    glyph.advance = (float) (bitmap_glyph->root.advance.x >> 16);
    if (bold)
        glyph.advance += (float) (weight) / float(1 << 6);

    glyph.lsb_delta = (int) (face->glyph->lsb_delta);
    glyph.rsb_delta = (int) (face->glyph->rsb_delta);

    Vec2i size(bitmap.width, bitmap.rows);

    if ((size.x > 0) && (size.y > 0)) {
        const int padding = 2;

        size += Vec2i(padding, padding) * 2;

        Page& page = load_page(char_size);

        glyph.texture_rect = find_glyph_rect(page, size);

        glyph.texture_rect.pos += Vec2i(padding, padding);
        glyph.texture_rect.size -= Vec2i(padding, padding) * 2;

        glyph.bounds.pos = Vec2f(Vec2i(bitmap_glyph->left, -bitmap_glyph->top));
        glyph.bounds.size = Vec2f(Vec2i(bitmap.width, bitmap.rows));

        pixel_buffer.resize(size.x * size.y * 4 );

        unsigned char* current = pixel_buffer.data();
        unsigned char* end = current + size.x * size.y * 4;

        while (current != end) {
            *current++ = 255;
            *current++ = 255;
            *current++ = 255;
            *current++ = 0;
        }

        const unsigned char* pixels = bitmap.buffer;
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
            for (int y = padding; y < size.y - padding; y++) {
                for (int x = padding; x < size.x - padding; x++) {
                    int index = x + y * size.x;
                    pixel_buffer[index * 4 + 3] = ((pixels[(x - padding) / 8]) & (1 << (7 - ((x - padding) % 8)))) ? 255 : 0;
                }
                pixels += bitmap.pitch;
            }
        }
        else {
            for (int y = padding; y < size.y - padding; y++) {
                for (int x = padding; x < size.x - padding; x++) {
                    int index = x + y * size.x;
                    pixel_buffer[index * 4 + 3] = pixels[x - padding];
                }
                pixels += bitmap.pitch;
            }
        }

        Vec2i dest = glyph.texture_rect.pos - Vec2i(padding, padding);
        Vec2i update_size = glyph.texture_rect.size + Vec2i(padding, padding) * 2;
        page.texture.update_sub(dest, update_size, pixel_buffer.data());
    }

    FT_Done_Glyph(glyph_desc);

    return glyph;
}

IntRect Font::find_glyph_rect(Page& page, Vec2i size) const {
    Row* row = nullptr;
    float best_ratio = 0;
    for (auto it = page.rows.begin(); it != page.rows.end() && !row; it++) {
        float ratio = (float) (size.y) / (float) (it->height);

        if ((ratio < 0.7f) || (ratio > 1.f))
            continue;

        if (size.x > page.texture.get_size().x - it->width)
            continue;

        if (ratio < best_ratio)
            continue;

        row = &*it;
        best_ratio = ratio;
    }

    if (!row) {
        int row_height = size.y + size.y / 10;
        while ((page.next_row + row_height >= page.texture.get_size().y) || (size.x >= page.texture.get_size().x)) {
            Vec2i texture_size = page.texture.get_size();
            if ((texture_size.x * 2 <= Texture::get_maximum_size()) && (texture_size.y * 2 <= Texture::get_maximum_size())) {
                page.texture.double_size();
            }
            else {
                EX_ERROR("Failed to add a new character to the font (the maximum texture size has been reached)");
                return { {0, 0}, {2, 2} };
            }
        }

        page.rows.emplace_back(page.next_row, row_height);
        page.next_row += row_height;
        row = &page.rows.back();
    }

    IntRect rect({ row->width, row->top }, size);

    row->width += size.x;

    return rect;
}

bool Font::set_current_size(unsigned int char_size) const {
    FT_Face face = font_handles->face;
    FT_UShort current_size = face->size->metrics.x_ppem;

    if (current_size != char_size) {
        FT_Error result = FT_Set_Pixel_Sizes(face, 0, char_size);

        if (result == FT_Err_Invalid_Pixel_Size) {
            if (!FT_IS_SCALABLE(face)) {
                std::stringstream stream;
                stream << "Failed to set bitmap font size to ";
                stream << char_size << '\n';
                stream << "Available sizes are: ";

                for (int i = 0; i < face->num_fixed_sizes; i++) {
                    long size = (face->available_sizes[i].y_ppem + 32) >> 6;
                    stream << size << " ";
                }

                EX_ERROR(stream.str());

            }
            else {
                EX_ERROR("Failed to set font size to " + std::to_string(char_size));
            }
        }

        return result == FT_Err_Ok;
    }

    return true;
}

Font::Page::Page() {
    Image image({ 128, 128 }, Color::Transparent);

    for (int x = 0; x < 2; x++)
        for (int y = 0; y < 2; y++)
            image.set_pixel({ x, y }, Color::White);

    if (!texture.load_from_image(image)) {
        EX_ERROR("Failed to load font page texture");
    }
}

Font::Page::Page(const Page& other) 
	: glyphs(other.glyphs), texture(other.texture.copy()), next_row(other.next_row), rows(other.rows) {}

}
