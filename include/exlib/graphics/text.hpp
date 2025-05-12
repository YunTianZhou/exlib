#pragma once

#include <string>

#include "exlib/graphics/drawable.hpp"
#include "exlib/graphics/transformable.hpp"
#include "exlib/graphics/types.hpp"

namespace ex {

class Font;

class EXLIB_API Text : public Drawable, public Transformable {
public:
    enum Style {
        Regular       = 0,
        Bold          = 1 << 0,
        Italic        = 1 << 1,
        Underlined    = 1 << 2,
        StrikeThrough = 1 << 3
    };

public:
    // Constructors and Destructors
    Text(const Font& _font, std::u32string _string = U"", unsigned int _char_size = 30);
    Text(const Font&& _font, std::u32string _string = U"", unsigned int _char_size = 30) = delete;
    virtual ~Text() override = default;

    // Copy and Move
    Text(Text&& other) noexcept = default;
    Text& operator=(Text&& other) noexcept = default;
    Text copy() const;

    // Setters
    void set_string(const std::u32string& _string);
    void set_font(const Font& _font);
    void set_font(const Font&& _font) = delete;
    void set_char_size(unsigned int size);
    void set_line_spacing(float spacing_factor);
    void set_letter_spacing(float spacing_factor);
    void set_style(unsigned int _style);
    void set_fill_color(Color color);
    void set_outline_color(Color color);
    void set_outline_thickness(float thickness);

    // Getters
    inline const std::u32string& get_string() const { return string; }
    inline const Font& get_font() const { return *font; }
    inline unsigned int get_char_size() const { return char_size; }
    inline float get_letter_spacing() const { return letter_spacing_factor; }
    inline float get_line_spacing() const { return line_spacing_factor; }
    inline unsigned int get_style() const { return style; }
    inline Color get_fill_color() const { return fill_color; }
    inline Color get_outline_color() const { return outline_color; }
    inline float get_outline_thickness() const { return outline_thickness; }

    Vec2f find_char_position(int index) const;
    FloatRect get_bounds() const;

private:
    Text(const Text& other) noexcept = default;
    void draw() const override;
    void update_geometry() const;

private:
    std::u32string string;
    const Font* font = nullptr;
    unsigned int char_size = 30;
    float letter_spacing_factor = 1.0f;
    float line_spacing_factor = 1.0f;
    unsigned int style = Regular;
    Color fill_color = Color::White;
    Color outline_color = Color::Black;
    float outline_thickness = 0.0f;
    mutable std::vector<Vertex> fill_vertices;
    mutable std::vector<Vertex> outline_vertices;
    mutable FloatRect bounds;
    mutable bool geometry_need_update = true;
};

}
