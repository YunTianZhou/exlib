#pragma once

#include "exlib/core/types.hpp"

namespace ex {

class EXLIB_API Drawable {
public:
	virtual void draw() const = 0;
	virtual ~Drawable() = default;
};

}