#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/vertex_buffer.hpp"

namespace ex::gl {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

VertexArray::VertexArray(VertexArray&& other)
    : id(other.id) {
    other.id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    if (this != &other) {
        glDeleteVertexArrays(1, &id);
        id = other.id;
        other.id = 0;
    }
    return *this;
}

void VertexArray::add_buffer(const VertexBuffer& buffer, const std::vector<Element>& elements) {
    bind();
    buffer.bind();

    GLuint stride = 0;
    for (const auto& element : elements) {
        stride += element.count * get_size_of_type(element.type);
    }

    GLuint offset = 0;
    for (GLuint i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            element.count,
            (GLenum) (element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            stride,
            (const void*) (uintptr_t) (offset)
        );
        offset += element.count * get_size_of_type(element.type);
    }
}

}
