#pragma once

#include <GL/glew.h>
#include "types.hpp"

namespace ex::gl {

class EXLIB_API VertexBuffer {
public:
    // Constructors and destructors
    explicit VertexBuffer(BufferUsage usage);
    VertexBuffer(BufferUsage usage, const void* data, GLsizei size);
    ~VertexBuffer();

    // Copy and Move
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer& operator=(VertexBuffer&& other);
    VertexBuffer copy() const;

    // Binding and unbinding
    inline void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }
    inline void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    // Getters
    inline BufferUsage get_usage() const { return usage; }
    inline GLsizei get_size() const { return size; }

    // Setters
    void set_data(const void* data, GLsizei _size);
    void update_sub_data(const void* data, GLuint offset, GLsizei _size);

    void* map(BufferAccess access);
    void unmap() const;

    // Clear buffer data
    void clear();

private:
    BufferUsage usage;
    GLuint id;
    GLsizei size;
};

}
