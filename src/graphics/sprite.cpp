#include <cmath>

#include "exlib/graphics/sprite.hpp"
#include "exlib/graphics/texture.hpp"
#include "exlib/graphics/draw.hpp"

namespace ex {

Sprite::Sprite(const Texture& _texture) 
	: Sprite(_texture, IntRect({0, 0}, _texture.get_size())) {}

Sprite::Sprite(const Texture& _texture, const IntRect& rect) 
	: texture(&_texture), texture_rect(rect) {
	update_vertices();
}

void Sprite::set_texture(const Texture& _texture, bool reset_rect) {
	if(reset_rect)
		set_texture_rect(IntRect({ 0, 0 }, _texture.get_size()));

	texture = &_texture;
}

void Sprite::set_texture_rect(const IntRect& rect) {
	if (texture_rect != rect) {
		texture_rect = rect;
		update_vertices();
	}
}

void Sprite::set_color(Color color) {
	Vertex* ptr = vertices;
	Vertex* end = vertices + 4;

	while (ptr != end) {
		(ptr++)->color = color;
	}
}

void Sprite::draw() const {
	const glm::mat4& transform = get_transform();

	Draw::State state = {
		PrimitiveType::TriangleStrip,
		&transform,
		texture
	};

	Draw::draw(vertices, 4, state);
}

void Sprite::update_vertices() {
	auto [pos, size] = FloatRect(texture_rect);
	Vec2f abs_size(std::abs(size.x), std::abs(size.y));

	vertices[0].pos = { 0.0f, 0.0f };
	vertices[1].pos = { 0.0f, abs_size.y };
	vertices[2].pos = { abs_size.x, 0.0f };
	vertices[3].pos = abs_size;

	vertices[0].tex_coords = pos;
	vertices[1].tex_coords = { pos.x, pos.y + size.y };
	vertices[2].tex_coords = { pos.x + size.x, pos.y };
	vertices[3].tex_coords = pos + size;
}

}
