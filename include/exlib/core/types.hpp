#pragma once

#include <string>
#include <cmath>

#include "config.hpp"

namespace ex {

template<typename T>
struct EXLIB_API Vec2 { T x, y; };

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

}