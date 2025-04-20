#pragma once

#include "exlib/graphics/shape.hpp"

namespace ex {

class ConvShape : public Shape {
public:
    explicit ConvShape(int point_count = 0) { set_point_count(point_count); }

    virtual ~ConvShape() = default;

    // Setters 
    inline void set_point_count(int point_count) { 
        points.resize(point_count); 
        update(); 
    }

    inline void set_point(int i, Vec2f pos) {
        if (i >= get_point_count())
            EX_THROW("Point index out of range: " + std::to_string(i));

        points[i] = pos; 
        update(); 
    }

    // Implemenatations
    inline int get_point_count() const override { return (int) points.size(); }

    inline Vec2f get_point(int i) const override {
        if (i >= get_point_count())
            EX_THROW("Point index out of range: " + std::to_string(i));

        return points[i];
    }

private:
    std::vector<Vec2f> points;

};

}