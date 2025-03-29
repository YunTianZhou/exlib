#include "exlib/opengl/index_buffer.hpp"

namespace ex::gl {

IndexBuffer::IndexBuffer(BufferUsage usage)
    : usage(usage), count(0) {
    glGenBuffers(1, &id);
}

IndexBuffer::IndexBuffer(BufferUsage usage, const GLuint* data, GLuint count)
    : usage(usage), count(count) {
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), data, (GLenum) (usage));
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
    : id(other.id), usage(other.usage), count(other.count) {
    other.id = 0;
    other.count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) {
    if (this != &other) {
        glDeleteBuffers(1, &id);
        id = other.id;
        usage = other.usage;
        count = other.count;

        other.id = 0;
        other.count = 0;
    }
    return *this;
}

IndexBuffer IndexBuffer::copy() const {
    IndexBuffer new_buffer(usage);
    new_buffer.set_data(nullptr, count);

    glBindBuffer(GL_COPY_READ_BUFFER, id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, new_buffer.id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, get_size());

    return new_buffer;
}

void IndexBuffer::set_data(const GLuint* data, GLuint _count) {
    count = _count;
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), data, (GLenum) (usage));
}

void IndexBuffer::update_sub_data(const GLuint* data, GLuint offset, GLuint size) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(GLuint), size * sizeof(GLuint), data);
}

GLuint* IndexBuffer::map(BufferAccess access) {
    bind();
    return (GLuint*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLenum) (access));
}

void IndexBuffer::unmap() const {
    bind();
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void IndexBuffer::clear() {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_size(), nullptr, (GLenum) (usage));
}

}
