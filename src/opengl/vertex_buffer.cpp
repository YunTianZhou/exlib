#include "exlib/opengl/vertex_buffer.hpp"

namespace ex::gl {

VertexBuffer::VertexBuffer(BufferUsage usage)
    : usage(usage), size(0) {
    glGenBuffers(1, &id);
}

VertexBuffer::VertexBuffer(BufferUsage usage, const void* data, GLuint size)
    : usage(usage), size(size) {
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum) (usage));
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
    : id(other.id), usage(other.usage), size(other.size) {
    other.id = 0;
    other.size = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    if (this != &other) {
        glDeleteBuffers(1, &id);
        id = other.id;
        usage = other.usage;
        size = other.size;

        other.id = 0;
        other.size = 0;
    }
    return *this;
}

VertexBuffer VertexBuffer::copy() const {
    VertexBuffer new_buffer(usage);
    new_buffer.set_data(nullptr, size);

    glBindBuffer(GL_COPY_READ_BUFFER, id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer.id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    return new_buffer;
}

void VertexBuffer::set_data(const void* data, GLuint _size) {
    size = _size;
    bind();
    glBufferData(GL_ARRAY_BUFFER, _size, data, (GLenum) (usage));
}

void VertexBuffer::update_sub_data(const void* data, GLuint offset, GLuint size) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void* VertexBuffer::map(BufferAccess access) {
    bind();
    return glMapBuffer(GL_ARRAY_BUFFER, (GLenum) access);
}

void VertexBuffer::unmap() const {
    bind();
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VertexBuffer::clear() {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, (GLenum) (usage));
}

}
