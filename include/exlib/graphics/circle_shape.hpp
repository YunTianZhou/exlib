#pragma once

#include <cmath>

#include "exlib/graphics/shape.hpp"

namespace ex {

class EXLIB_API CircleShape : public Shape {
public:
    explicit CircleShape(float _radius, int _point_count = 30)
        : radius(_radius), point_count(_point_count) {
        update();
    }

    virtual ~CircleShape() = default;

    // Setters 
    inline void set_radius(float _radius) { radius = _radius; update(); }

    // Getters
    inline float get_radius() const { return radius; }

    // Implemenatations
    inline int get_point_count() const override { return point_count; };

    inline Vec2f get_point(int i) const override {
        constexpr float PI = 3.14159;

        float angle = i * 2.0f * PI / point_count - PI / 2.0f;
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        return { radius + x, radius + y };
    }

    inline Vec2f get_geometric_center() const override {
        return { radius, radius };
    }

private:
    float radius;
    int point_count;

};

}