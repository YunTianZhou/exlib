#include <algorithm>

#include "exlib/graphics/shape.hpp"
#include "exlib/graphics/draw.hpp"
#include "exlib/graphics/texture.hpp"

namespace ex {

inline static Vec2f compute_normal(Vec2f p1, Vec2f p2) {
	Vec2f v = (p2 - p1).perpendicular();
	float mag = v.magnitude();
	if (mag == 0.0f) {
		return v;
	}
	return v / mag;
}

inline static FloatRect compute_bounds(const std::vector<Vertex>& vertices) {
	if (vertices.empty()) {
		return FloatRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	float min_x = vertices[0].pos.x;
	float min_y = vertices[0].pos.y;
	float max_x = vertices[0].pos.x;
	float max_y = vertices[0].pos.y;

	for (const auto& vertex : vertices) {
		if (vertex.pos.x < min_x) min_x = vertex.pos.x;
		if (vertex.pos.y < min_y) min_y = vertex.pos.y;
		if (vertex.pos.x > max_x) max_x = vertex.pos.x;
		if (vertex.pos.y > max_y) max_y = vertex.pos.y;
	}

	return FloatRect(min_x, min_y, max_x - min_x, max_y - min_y);
}

void Shape::set_texture(const Texture* _texture, bool reset_rect) {
	if (_texture) {
		if (reset_rect || (!texture && (texture_rect == IntRect{})))
			set_texture_rect(IntRect({ 0, 0 }, Vec2i(_texture->get_size())));
	}

	texture = _texture;
}

void Shape::set_texture_rect(const IntRect& rect) {
	texture_rect = rect;
	update_tex_coords();
}

void Shape::set_fill_color(Color color) {
	fill_color = color;
	update_fill_color();
}

void Shape::set_outline_color(Color color) {
	outline_color = color;
	update_outline_color();
}

void Shape::set_outline_thickness(float thickness) {
	outline_thickness = thickness;
	update();
}

Vec2f Shape::get_geometric_center() const {
	int count = get_point_count();

	switch (count) {
	case 0:
		throw Exception("Cannot get geometric center of empty shape.");
	case 1:
		return get_point(0);
	case 2:
		return (get_point(0) + get_point(1)) / 2;
	default:
		Vec2f centroid;
		float twice_area = 0;

		Vec2f prev = get_point(count - 1);
		for (int i = 0; i < count; i++) {
			Vec2f curr = get_point(i);
			float product = prev.cross(curr);
			twice_area += product;
			centroid += (curr + prev) * product;

			prev = curr;
		}

		if (twice_area != 0.f) {
			return centroid / 3.f / twice_area;
		}

		Vec2f min_point = get_point(0);
		Vec2f max_point = min_point;
		for (int i = 1; i < count; i++) {
			Vec2f curr = get_point(i);
			min_point.x = std::min(min_point.x, curr.x);
			min_point.y = std::min(min_point.y, curr.y);
			max_point.x = std::max(max_point.x, curr.x);
			max_point.y = std::max(max_point.y, curr.y);
		}

		return (max_point + min_point) / 2.0f;
	}
}

void Shape::update() {
	int count = get_point_count();
	if (count < 3) {
		fill_vertices.clear();
		outline_vertices.clear();
		return;
	}

	fill_vertices.resize(count + 2);
	for (int i = 0; i < count; i++) {
		fill_vertices[i + 1].pos = get_point(i);
	}
	fill_vertices[count + 1].pos = fill_vertices[1].pos;

	fill_vertices[0] = fill_vertices[1];
	inside_bounds = compute_bounds(fill_vertices);
	fill_vertices[0].pos = inside_bounds.get_center();
	
	update_fill_color();
	update_tex_coords();
	update_outline();
}

void Shape::draw() const {
	const glm::mat4& transform = get_transform();

	Draw::State state = {
		PrimitiveType::TriangleFan,
		&transform,
		texture
	};

	if (get_point_count() >= 3 && !fill_vertices.empty()) {
		Draw::draw(fill_vertices, state);
	}

	if (outline_thickness != 0.0f && !outline_vertices.empty()) {
		state.type = PrimitiveType::TriangleStrip;
		state.texture = nullptr;
		Draw::draw(outline_vertices, state);
	}
}

void Shape::update_fill_color() {
	for (auto& vertex : fill_vertices) {
		vertex.color = fill_color;
	}
}

void Shape::update_tex_coords() {
	FloatRect rect(texture_rect);

	Vec2f inside_size(
		inside_bounds.size.x > 0 ? inside_bounds.size.x : 1.0f,
		inside_bounds.size.y > 0 ? inside_bounds.size.y : 1.0f
	);

	for (auto& vertex : fill_vertices) {
		Vec2f ratio = (vertex.pos - inside_bounds.pos) / inside_size;
		vertex.tex_coords = rect.pos + ratio * rect.size;
	}
}

void Shape::update_outline() {
	if (outline_thickness == 0.0f) {
		outline_vertices.clear();
		outline_bounds = inside_bounds;
		return;
	}

	int count = (int) (fill_vertices.size() - 2);
	outline_vertices.resize((count + 1) * 2);

	for (int i = 0; i < count; i++) {
		int index = i + 1;

		Vec2f p0 = (i == 0) ? fill_vertices[count].pos : fill_vertices[index - 1].pos;
		Vec2f p1 = fill_vertices[index].pos;
		Vec2f p2 = fill_vertices[index + 1].pos;

		Vec2f n1 = compute_normal(p0, p1);
		Vec2f n2 = compute_normal(p1, p2);

		if (n1.dot(fill_vertices[0].pos - p1) > 0) n1 = -n1;
		if (n2.dot(fill_vertices[0].pos - p1) > 0) n2 = -n2;

		float factor = 1.0f + (n1.x * n2.x + n1.y * n2.y);
		Vec2f normal = (n1 + n2) / factor;

		outline_vertices[i * 2 + 0].pos = p1;
		outline_vertices[i * 2 + 1].pos = p1 + normal * outline_thickness;
	}

	outline_vertices[count * 2 + 0].pos = outline_vertices[0].pos;
	outline_vertices[count * 2 + 1].pos = outline_vertices[1].pos;

	update_outline_color();

	outline_bounds = compute_bounds(outline_vertices);
}

void Shape::update_outline_color() {
	for (auto& vertex : outline_vertices) {
		vertex.color = outline_color;
	}
}

}