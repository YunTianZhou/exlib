#pragma once

#include <string>
#include <cmath>

#include "config.hpp"

namespace ex {

template <class T>
struct EXLIB_API Vec2 {
    T x, y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}

    // Addition
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }

    // Subtraction
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }

    // Scalar multiplication
    Vec2 operator*(T scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }

    // Scalar division
    Vec2 operator/(T scalar) const { return Vec2(x / scalar, y / scalar); }
    Vec2& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

    // Dot product
    T dot(const Vec2& other) const { return x * other.x + y * other.y; }

    // Magnitude
    T magnitude() const { return std::sqrt(x * x + y * y); }

    // Normalization
    Vec2 normalized() const {
        T mag = magnitude();
        return (mag == 0) ? Vec2(0, 0) : Vec2(x / mag, y / mag);
    }

    // Equality check
    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }
};

template <class T>
struct EXLIB_API Vec3 {
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    // Addition
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }

    // Subtraction
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    // Scalar multiplication
    Vec3 operator*(T scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }

    // Scalar division
    Vec3 operator/(T scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
    Vec3& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    // Dot product
    T dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }

    // Cross product
    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude
    T magnitude() const { return std::sqrt(x * x + y * y + z * z); }

    // Normalization
    Vec3 normalized() const {
        T mag = magnitude();
        return (mag == 0) ? Vec3(0, 0, 0) : Vec3(x / mag, y / mag, z / mag);
    }

    // Equality check
    bool operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vec3& other) const { return !(*this == other); }
};

struct EXLIB_API Color {
    unsigned char r, g, b, a = 255;

    Color() = default; 

    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    Color(unsigned int color)
        : r((color >> 24) & 0xFF), g((color >> 16) & 0xFF), b((color >> 8) & 0xFF), a(color & 0xFF) {}

    inline int to_int() const { return (int) ((a << 24) | (r << 16) | (g << 8) | b); }

    // Addition (saturates at 255)
    inline Color operator+(const Color& other) const {
        return Color(
            (unsigned char) (std::min(255, (int) (r) + (int) (other.r))),
            (unsigned char) (std::min(255, (int) (g) + (int) (other.g))),
            (unsigned char) (std::min(255, (int) (b) + (int) (other.b))),
            (unsigned char) (std::min(255, (int) (a) + (int) (other.a)))
        );
    }

    inline Color& operator+=(const Color& other) {
        r = (unsigned char) (std::min(255, (int) (r) + (int) (other.r)));
        g = (unsigned char) (std::min(255, (int) (g) + (int) (other.g)));
        b = (unsigned char) (std::min(255, (int) (b) + (int) (other.b)));
        a = (unsigned char) (std::min(255, (int) (a) + (int) (other.a)));
        return *this;
    }

    // Subtraction (clamps at 0)
    inline Color operator-(const Color& other) const {
        return Color(
            (unsigned char) (std::max(0, (int) (r) - (int) (other.r))),
            (unsigned char) (std::max(0, (int) (g) - (int) (other.g))),
            (unsigned char) (std::max(0, (int) (b) - (int) (other.b))),
            (unsigned char) (std::max(0, (int) (a) - (int) (other.a)))
        );
    }

    inline Color& operator-=(const Color& other) {
        r = (unsigned char) (std::max(0, (int) (r) - (int) (other.r)));
        g = (unsigned char) (std::max(0, (int) (g) - (int) (other.g)));
        b = (unsigned char) (std::max(0, (int) (b) - (int) (other.b)));
        a = (unsigned char) (std::max(0, (int) (a) - (int) (other.a)));
        return *this;
    }

    // Multiplication (component-wise, multiplies each channel and divides by 255)
    inline Color operator*(const Color& other) const {
        return Color(
            (unsigned char) (((int) (r) * (int) (other.r)) / 255),
            (unsigned char) (((int) (g) * (int) (other.g)) / 255),
            (unsigned char) (((int) (b) * (int) (other.b)) / 255),
            (unsigned char) (((int) (a) * (int) (other.a)) / 255)
        );
    }

    inline Color& operator*=(const Color& other) {
        r = (unsigned char) (((int) (r) * (int) (other.r)) / 255);
        g = (unsigned char) (((int) (g) * (int) (other.g)) / 255);
        b = (unsigned char) (((int) (b) * (int) (other.b)) / 255);
        a = (unsigned char) (((int) (a) * (int) (other.a)) / 255);
        return *this;
    }

    // Equality check
    inline bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    inline bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    // Default colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

}