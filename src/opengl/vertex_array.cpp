#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/vertex_buffer.hpp"

namespace ex::gl {

VertexArray::VertexArray()
    : id(0), stride(0), bound_buffer(nullptr) {
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

VertexArray::VertexArray(VertexArray&& other)
    : id(other.id), stride(other.stride), bound_buffer(other.bound_buffer) {
    other.id = 0;
    other.stride = 0;
    other.bound_buffer = nullptr;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    if (this != &other) {
        glDeleteVertexArrays(1, &id);
        id = other.id;
        stride = other.stride;
        bound_buffer = other.bound_buffer;
        other.id = 0;
        other.stride = 0;
        other.bound_buffer = nullptr;
    }
    return *this;
}

void VertexArray::set_layout(const VertexBuffer& buffer, const std::vector<Element>& layout) {
    bind();
    buffer.bind();

    stride = 0;
    for (const auto& element : layout) {
        stride += element.count * get_size_of_type(element.type);
    }

    GLuint offset = 0;
    for (GLuint i = 0; i < layout.size(); i++) {
        const auto& element = layout[i];
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

    bound_buffer = &buffer;
}

GLsizei VertexArray::get_count() const {
    if (!bound_buffer)
        throw Exception("No buffer bound.");

    if (stride == 0)
        throw Exception("Stride is zero, cannot compute count.");

    return (GLsizei) (bound_buffer->get_size() / stride);
}

}
