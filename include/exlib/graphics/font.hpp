#pragma once

#include <string>
#include <string_view>
#include <istream>
#include <unordered_map>
#include <filesystem>

#include "exlib/graphics/texture.hpp"
#include "exlib/graphics/types.hpp"

namespace ex {

struct EXLIB_API Glyph {
    float advance = 0.0f;
    int lsb_delta = 0;
    int rsb_delta = 0;
    FloatRect bounds;
    IntRect texture_rect;
};

class EXLIB_API Font {
public:
    struct Info {
        std::string family;
    };

    // Constructors
    Font() = default;
    explicit Font(const std::filesystem::path& path);
    Font(const void* data, int size);

    // Copy and Move
    Font(Font&& other) noexcept = default;
    Font& operator=(Font&& other) noexcept = default;
    Font copy() const;

    // Loaders
    bool open_from_file(const std::filesystem::path& path);
    bool open_from_memory(const void* data, int size);

    // Getters and Setters
    const Info& get_info() const;

    bool has_glyph(char32_t code_point) const;
    const Glyph& get_glyph(char32_t code_point, unsigned int char_size, bool bold, float outline_thickness = 0) const;

    float get_kerning(unsigned int first, unsigned int second, unsigned int char_size, bool bold = false) const;

    float get_line_spacing(unsigned int char_size) const;

    float get_underline_position(unsigned int char_size) const;
    float get_underline_thickness(unsigned int char_size) const;

    const Texture& get_texture(unsigned int char_size) const;

private:
    Font(const Font& other);

private:
    struct Row {
        Row(unsigned int row_top, unsigned int row_height)
            : top(row_top), height(row_height) {
        }

        int width = 0;
        int top;
        int height;
    };

    using GlyphTable = std::unordered_map<unsigned long long, Glyph>;

    struct Page {
        explicit Page();
        Page(const Page& other);

        GlyphTable glyphs;
        Texture texture;
        int next_row = 3;
        std::vector<Row> rows;
    };

    void cleanup();
    bool open_from_stream_impl(std::istream& stream, std::string type);
    Page& load_page(unsigned int char_size) const;
    Glyph load_glyph(char32_t code_point, unsigned int char_size, bool bold, float outline_thickness) const;
    IntRect find_glyph_rect(Page& page, Vec2i size) const;
    bool set_current_size(unsigned int char_size) const;

    struct FontHandles;
    using PageTable = std::unordered_map<unsigned int, Page>;

private:
    std::shared_ptr<FontHandles> font_handles;
    Info info;
    mutable PageTable pages;
    mutable std::vector<unsigned char> pixel_buffer;
    std::shared_ptr<std::istream> stream_;
};

}