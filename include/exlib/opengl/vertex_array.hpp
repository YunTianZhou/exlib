#pragma once

#include <vector>

#include <GL/glew.h>
#include "types.hpp"

namespace ex::gl {

class VertexBuffer;

class EXLIB_API VertexArray {
public:
    struct Element {
        GLuint count;
        Type type;
        bool normalized;
    };

public:
    // Constructors and destructors
    VertexArray();
    ~VertexArray();

    // Copy and Move
    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other);
    VertexArray& operator=(VertexArray&& other);

    // Binding and unbinding
    inline void bind() const { glBindVertexArray(id); }
    inline void unbind() const { glBindVertexArray(0); }

    // Set layout
    void set_layout(const VertexBuffer& buffer, const std::vector<Element>& layout);

    // Getters
    inline const VertexBuffer* get_bound_buffer() const { return bound_buffer; } ;

private:
    GLuint id;
    const VertexBuffer* bound_buffer;
};

}
