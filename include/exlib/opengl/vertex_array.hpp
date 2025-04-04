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

    // Add buffer
    void add_buffer(const VertexBuffer& buffer, const std::vector<Element>& elements);

    // Getters
    inline GLsizei get_count() const { return count; }

private:
    GLuint id;
    GLsizei count;
};

}
