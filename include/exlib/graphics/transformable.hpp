#pragma once

#include <glm/glm.hpp>

#include "exlib/core/types.hpp"

namespace ex {

class EXLIB_API Transformable {
public:
	Transformable() = default;
	virtual ~Transformable() = default;

	// Setters
	void set_origin(Vec2f _origin);
	void set_position(Vec2f _pos);
	void set_rotation(float _rotation);
	void set_scale(Vec2f _scale);

	// Getters
	inline Vec2f get_origin() const { return origin; }
	inline Vec2f get_position() const { return pos; }
	inline float get_rotation() const { return rotation; }
	inline Vec2f get_scale() const { return scale_; }

	// Transformations
	void move(Vec2f offset);
	void rotate(float angle);
	void scale(Vec2f factor);

	// Matrices
	const glm::mat4& get_transform() const;
	const glm::mat4& get_inverse_transform() const;

private:
	Vec2f origin;
	Vec2f pos;
	float rotation = 0.0f;
	Vec2f scale_ = { 1.0f, 1.0f };

	mutable glm::mat4 transform_matrix;
	mutable glm::mat4 inverse_transform_matrix;
	mutable bool needs_update = true;
	mutable bool needs_update_inverse = true;
};

}