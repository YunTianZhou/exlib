#include <algorithm>
#include <cmath>

#include <glm/glm.hpp>

#include "exlib/graphics/draw.hpp"
#include "exlib/graphics/text.hpp"
#include "exlib/graphics/font.hpp"

namespace ex {

inline static void add_line(std::vector<Vertex>& vertices,
                            float                line_length,
                            float                line_top,
                            Color                color,
                            float                offset,
                            float                thickness,
                            float                outline_thickness = 0) {
    float top = std::floor(line_top + offset - (thickness / 2) + 0.5f);
    float bottom = top + std::floor(thickness + 0.5f);

    vertices.emplace_back(Vec2f(-outline_thickness, top - outline_thickness), color, Vec2f {1.0f, 1.0f});
    vertices.emplace_back(Vec2f(line_length + outline_thickness, top - outline_thickness), color, Vec2f {1.0f, 1.0f});
    vertices.emplace_back(Vec2f(-outline_thickness, bottom + outline_thickness), color, Vec2f {1.0f, 1.0f});
    vertices.emplace_back(Vec2f(-outline_thickness, bottom + outline_thickness), color, Vec2f {1.0f, 1.0f});
    vertices.emplace_back(Vec2f(line_length + outline_thickness, top - outline_thickness), color, Vec2f {1.0f, 1.0f});
    vertices.emplace_back(Vec2f(line_length + outline_thickness, bottom + outline_thickness), color, Vec2f {1.0f, 1.0f});
}

inline static void add_glyph_quad(std::vector<Vertex>& vertices, 
                                  Vec2f                pos, 
                                  Color                color, 
                                  const Glyph&         glyph, 
                                  float                italic_shear) {
    Vec2f padding(1.0f, 1.0f);

    Vec2f p1 = glyph.bounds.pos - padding;
    Vec2f p2 = glyph.bounds.pos + glyph.bounds.size + padding;

    Vec2f uv1 = Vec2f(glyph.texture_rect.pos) - padding;
    Vec2f uv2 = Vec2f(glyph.texture_rect.pos + glyph.texture_rect.size) + padding;

    vertices.emplace_back(pos + Vec2f(p1.x - italic_shear * p1.y, p1.y), color, Vec2f(uv1.x, uv1.y));
    vertices.emplace_back(pos + Vec2f(p2.x - italic_shear * p1.y, p1.y), color, Vec2f(uv2.x, uv1.y));
    vertices.emplace_back(pos + Vec2f(p1.x - italic_shear * p2.y, p2.y), color, Vec2f(uv1.x, uv2.y));
    vertices.emplace_back(pos + Vec2f(p1.x - italic_shear * p2.y, p2.y), color, Vec2f(uv1.x, uv2.y));
    vertices.emplace_back(pos + Vec2f(p2.x - italic_shear * p1.y, p1.y), color, Vec2f(uv2.x, uv1.y));
    vertices.emplace_back(pos + Vec2f(p2.x - italic_shear * p2.y, p2.y), color, Vec2f(uv2.x, uv2.y));
}

Text::Text(const Font& _font, std::u32string _string, unsigned int _char_size)
	: string(std::move(_string)), font(&_font), char_size(_char_size) {}

Text Text::copy() const {
    return *this;
}

void Text::set_string(const std::u32string& _string) {
    if (string != _string) {
        string = _string;
        geometry_need_update = true;
    }
}

void Text::set_font(const Font& _font) {
    if (font != &_font) {
        font = &_font;
        geometry_need_update = true;
    }
}

void Text::set_char_size(unsigned int size) {
    if (char_size != size) {
        char_size = size;
        geometry_need_update = true;
    }
}

void Text::set_line_spacing(float spacing_factor) {
    if (letter_spacing_factor != spacing_factor) {
        letter_spacing_factor = spacing_factor;
        geometry_need_update = true;
    }
}

void Text::set_letter_spacing(float spacing_factor) {
    if (line_spacing_factor != spacing_factor) {
        line_spacing_factor = spacing_factor;
        geometry_need_update = true;
    }
}

void Text::set_style(unsigned int _style) {
    if (style != _style) {
        style = _style;
        geometry_need_update = true;
    }
}

void Text::set_fill_color(Color color) {
    if (color != fill_color) {
        fill_color = color;

        if (!geometry_need_update) {
            for (auto& vertex : fill_vertices)
                vertex.color = fill_color;
        }
    }
}

void Text::set_outline_color(Color color) {
    if (color != outline_color) {
        outline_color = color;

        if (!geometry_need_update) {
            for (auto& vertex : outline_vertices)
                vertex.color = outline_color;
        }
    }
}

void Text::set_outline_thickness(float thickness) {
    if (thickness != outline_thickness) {
        outline_thickness = thickness;
        geometry_need_update = true;
    }
}

Vec2f Text::find_char_position(int index) const {
    index = std::min(index, (int) string.size());

    bool is_bold = style & Bold;
    float whitespace_width = font->get_glyph(U' ', char_size, is_bold).advance;
    float letter_spacing = (whitespace_width / 3.0f) * (letter_spacing_factor - 1.0f);
    whitespace_width += letter_spacing;
    float line_spacing = font->get_line_spacing(char_size) * line_spacing_factor;

    Vec2f pos;
    unsigned int prev_char = 0;
    for (int i = 0; i < index; i++) {
        unsigned int curr_char = string[i];

        pos.x += font->get_kerning(prev_char, curr_char, char_size, is_bold);
        prev_char = curr_char;

        switch (curr_char) {
        case U' ':
            pos.x += whitespace_width;
            continue;
        case U'\t':
            pos.x += whitespace_width * 4;
            continue;
        case U'\n':
            pos.y += line_spacing;
            pos.x = 0;
            continue;
        }

        pos.x += font->get_glyph(curr_char, char_size, is_bold).advance + letter_spacing;
    }

    return pos;
}

FloatRect Text::get_bounds() const {
    update_geometry();

    return bounds;
}

void Text::draw() const {
    update_geometry();

    const glm::mat4& transform = get_transform();

    Draw::State state = {
        PrimitiveType::Triangles,
        &transform,
        &font->get_texture(char_size)
    };

    if (outline_thickness != 0)
        Draw::draw(outline_vertices, state);

    Draw::draw(fill_vertices, state);
}

void Text::update_geometry() const {
    if (!geometry_need_update)
        return;

    geometry_need_update = false;

    fill_vertices.clear();
    outline_vertices.clear();
    bounds = FloatRect();

    if (string.empty())
        return;

    bool is_bold = style & Bold;
    bool is_underlined = style & Underlined;
    bool is_strike_through = style & StrikeThrough;
    float italic_shear = (style & Italic) ? glm::radians(12.0f) : 0.0f;
    float underline_offset = font->get_underline_position(char_size);
    float underline_thickness = font->get_underline_thickness(char_size);

    float strike_through_offset = font->get_glyph(U'x', char_size, is_bold).bounds.get_center().y;

    float whitespace_width = font->get_glyph(U' ', char_size, is_bold).advance;
    float letter_spacing = (whitespace_width / 3.0f) * (letter_spacing_factor - 1.0f);
    whitespace_width += letter_spacing;
    float line_spacing = font->get_line_spacing(char_size) * line_spacing_factor;
    float x = 0.0f;
    float y = (float) (char_size);

    float min_x = (float) (char_size);
    float min_y = (float) (char_size);
    float max_x = 0.0f;
    float max_y = 0.0f;
    unsigned int prev_char = 0;
    for (unsigned int curr_char : string) {
        if (curr_char == U'\r')
            continue;

        x += font->get_kerning(prev_char, curr_char, char_size, is_bold);

        if (is_underlined && (curr_char == U'\n' && prev_char != U'\n')) {
            add_line(fill_vertices, x, y, fill_color, underline_offset, underline_thickness);

            if (outline_thickness)
                add_line(outline_vertices, x, y, outline_color, underline_offset, underline_thickness, outline_thickness);
        }

        if (is_strike_through && (curr_char == U'\n' && prev_char != U'\n')) {
            add_line(fill_vertices, x, y, fill_color, strike_through_offset, underline_thickness);

            if (outline_thickness)
                add_line(outline_vertices, x, y, outline_color, strike_through_offset, underline_thickness, outline_thickness);
        }

        prev_char = curr_char;

        if ((curr_char == U' ') || (curr_char == U'\n') || (curr_char == U'\t')) {
            min_x = std::min(min_x, x);
            min_y = std::min(min_y, y);

            switch (curr_char) {
            case U' ':
                x += whitespace_width;
                break;
            case U'\t':
                x += whitespace_width * 4;
                break;
            case U'\n':
                y += line_spacing;
                x = 0;
                break;
            }

            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);

            continue;
        }

        if (outline_thickness) {
            const Glyph& glyph = font->get_glyph(curr_char, char_size, is_bold, outline_thickness);
            add_glyph_quad(outline_vertices, Vec2f(x, y), outline_color, glyph, italic_shear);
        }

        const Glyph& glyph = font->get_glyph(curr_char, char_size, is_bold);
        add_glyph_quad(fill_vertices, Vec2f(x, y), fill_color , glyph, italic_shear);

        Vec2f p1 = glyph.bounds.pos;
        Vec2f p2 = glyph.bounds.pos + glyph.bounds.size;

        min_x = std::min(min_x, x + p1.x - italic_shear * p2.y);
        max_x = std::max(max_x, x + p2.x - italic_shear * p1.y);
        min_y = std::min(min_y, y + p1.y);
        max_y = std::max(max_y, y + p2.y);

        x += glyph.advance + letter_spacing;
    }

    if (outline_thickness) {
        float outline = std::abs(std::ceil(outline_thickness));
        min_x -= outline;
        max_x += outline;
        min_y -= outline;
        max_y += outline;
    }

    if (is_underlined && (x > 0)) {
        add_line(fill_vertices, x, y, fill_color, underline_offset, underline_thickness);

        if (outline_thickness)
            add_line(outline_vertices, x, y, outline_color, underline_offset, underline_thickness, outline_thickness);
    }

    if (is_strike_through && (x > 0)) {
        add_line(fill_vertices, x, y, fill_color, strike_through_offset, underline_thickness);

        if (outline_thickness)
            add_line(outline_vertices, x, y, outline_color, strike_through_offset, underline_thickness, outline_thickness);
    }

    bounds.pos = Vec2f(min_x, min_y);
    bounds.size = Vec2f(max_x, max_y) - Vec2f(min_x, min_y);
}

}
