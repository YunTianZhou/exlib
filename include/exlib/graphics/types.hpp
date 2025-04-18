#pragma once

#include <vector>
#include <algorithm>

#include "exlib/core/types.hpp"
#include "exlib/opengl/types.hpp"

namespace ex {

using PrimitiveType = gl::PrimitiveType;

struct EXLIB_API Vertex {
	Vec2f pos;
	Color color;

	Vertex() : pos(0.0f, 0.0f), color(Color::Transparent) {}

	Vertex(const Vec2f& _pos, const Color& _color)
		: pos(_pos), color(_color) {
	}
};

template <class T>
struct EXLIB_API Rect {
	Vec2<T> pos, size;

	Rect() = default;

	Rect(Vec2<T> _pos, Vec2<T> _size) : pos(_pos), size(_size) {}

	Rect(T x, T y, T w, T h) : pos(x, y), size(w, h) {}

	template <class U>
	inline explicit operator Rect<U> () const {
		return Rect<U>(Vec2<U>(pos), Vec2<U>(size));
	}

	inline bool contains(Vec2<T> point) const {
		T left = std::min(pos.x, pos.x + size.x);
		T right = std::max(pos.x, pos.x + size.x);
		T top = std::min(pos.y, pos.y + size.y);
		T bottom = std::max(pos.y, pos.y + size.y);

		return point.x >= left && point.x <= right && point.y >= top && point.y <= bottom;
	}

	inline Vec2<T> get_center() const {
		return pos + size / T(2);
	}

	inline bool operator==(const Rect<T>& other) const {
		return pos == other.pos && size == other.size;
	}

	inline bool operator!=(const Rect<T>& other) const  {
		return !(*this == other);
	}

};

using IntRect = Rect<int>;
using FloatRect = Rect<float>;

}