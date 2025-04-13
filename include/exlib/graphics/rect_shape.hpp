#include "exlib/graphics/shape.hpp"

namespace ex {

class EXLIB_API RectShape : public Shape {
public:
    RectShape(Vec2f _size = {}) { set_size(_size); }

    virtual ~RectShape() = default;

    // Setters 
    inline void set_size(Vec2f _size) { size = _size; update(); }

    // Getters
    inline Vec2f get_size() const { return size; };

    // Implemenatations
    inline int get_point_count() const override { return 4; };

    inline Vec2f get_point(int i) const override {
        switch (i) {
        default:
        case 0:
            return { 0.0f, 0.0f };
        case 1:
            return { size.x, 0.0f };
        case 2:
            return { size.x, size.y };
        case 3:
            return { 0.0f, size.y };
        }
    }

    Vec2f get_geometric_center() const override {
        return size / 2.0f;
    }

private:
    Vec2f size;

};

}