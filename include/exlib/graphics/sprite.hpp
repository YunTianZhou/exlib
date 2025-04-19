#pragma once

#include "exlib/graphics/draw.hpp"
#include "exlib/graphics/transformable.hpp"

namespace ex {

class Texture;

class EXLIB_API Sprite : public Drawable, public Transformable {
public:
    // Constructors
    explicit Sprite(const Texture& _texture);
    explicit Sprite(const Texture&& _texture) = delete;
    Sprite(const Texture& _texture, const IntRect& rect);
    Sprite(const Texture&& _texture, const IntRect& rect) = delete;

    // Setters
    void set_texture(const Texture& _texture, bool reset_rect = false);
    void set_texture(Texture&& _texture, bool reset_rect = false) = delete;
    void set_texture_rect(const IntRect& rect);
    void set_color(Color color);

    // Getters
    const Texture& get_texture() const { return *texture; }
    IntRect get_texture_rect() const { return texture_rect; }
    Color get_color() const { return vertices[0].color; }
    FloatRect get_bounds() const { return FloatRect { Vec2f {0.0f, 0.0f}, vertices[3].pos }; }

    // Draw
   void draw() const;

public:
    void update_vertices();

private:
	Vertex vertices[4];
	const Texture* texture;
	IntRect texture_rect;
};

}