#pragma once

#include "exlib/graphics/drawable.hpp"
#include "exlib/graphics/transformable.hpp"
#include "exlib/graphics/types.hpp"

namespace ex {

namespace gl {
    class VertexArray;
    class VertexBuffer;
}

class EXLIB_API Shape : public Drawable, public Transformable {
public:
    Shape() = default;
    virtual ~Shape() override = default;

    // Setters
    void set_fill_color(Color color);
    void set_outline_color(Color color);
    void set_outline_thickness(float thickness);

    // Getters
    inline Color get_fill_color() const { return fill_color; }
    inline Color get_outline_color() const { return outline_color; }
    inline float get_outline_thickness() const { return outline_thickness; }
    inline FloatRect get_bounds() const { return outline_bounds; }

    // Interfaces
    virtual int get_point_count() const = 0;
    virtual Vec2f get_point(int i) const = 0;
    virtual Vec2f get_geometric_center() const;

protected:
    void update();

private:
    // Override draw function from Drawable
    virtual void draw() const override;

    // Updaters
    void update_fill_color();
    void update_outline();
    void update_outline_color();

private:
    Color fill_color = Color::White;
    Color outline_color = Color::White;
    float outline_thickness = 0.0f;

    std::vector<Vertex> fill_vertices;
    std::vector<Vertex> outline_vertices;

    FloatRect inside_bounds;
    FloatRect outline_bounds;
};

}