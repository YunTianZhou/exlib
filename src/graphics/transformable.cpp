#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include "exlib/graphics/transformable.hpp"

namespace ex {

void Transformable::set_origin(Vec2f _origin) {
	origin = _origin;
	needs_update = true;
	needs_update_inverse = true;
}

void Transformable::set_position(Vec2f _pos) {
	pos = _pos;
	needs_update = true;
	needs_update_inverse = true;
}

void Transformable::set_rotation(float _rotation) {
	rotation = std::fmod(_rotation, 360.0f);
	if (rotation < 0.0f) {
		rotation += 360.0f;
	}
	needs_update = true;
	needs_update_inverse = true;
}

void Transformable::set_scale(Vec2f _scale) {
	scale_ = _scale;
	needs_update = true;
	needs_update_inverse = true;
}

void Transformable::move(Vec2f offset) {
	set_position(pos + offset);
}

void Transformable::rotate(float angle) {
	set_rotation(rotation + angle);
}

void Transformable::scale(Vec2f factor) {
	set_scale(Vec2f(scale_ * factor));
}

const glm::mat4& Transformable::get_transform() const {
	if (needs_update) {
		transform_matrix = glm::mat4(1.0f);
		transform_matrix = glm::translate(transform_matrix, glm::vec3(pos.x + origin.x, pos.y + origin.y, 0.0f));
		transform_matrix = glm::rotate(transform_matrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		transform_matrix = glm::scale(transform_matrix, glm::vec3(scale_.x, scale_.y, 1.0f));
		transform_matrix = glm::translate(transform_matrix, glm::vec3(-origin.x, -origin.y, 0.0f));

		needs_update = false;
	}

	return transform_matrix;
}

const glm::mat4& Transformable::get_inverse_transform() const {
	if (needs_update_inverse) {
		inverse_transform_matrix = glm::inverse(get_transform());
		needs_update_inverse = false;
	}
	
	return inverse_transform_matrix;
}

}