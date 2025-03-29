#pragma once

#include <GL/glew.h>
#include "types.hpp"

namespace ex::gl {

class EXLIB_API IndexBuffer {
public:
    // Constructors and destructors
    explicit IndexBuffer(BufferUsage usage);
    IndexBuffer(BufferUsage usage, const GLuint* data, GLuint count);
    ~IndexBuffer();

    // Copy and Move
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other);
    IndexBuffer& operator=(IndexBuffer&& other);
    IndexBuffer copy() const;

    // Binding and unbinding
    inline void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    inline void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    // Getters
    inline BufferUsage get_usage() const { return usage; }
    inline GLuint get_count() const { return count; }
    inline GLuint get_size() const { return sizeof(GLuint) * count; }
    inline GLuint get_id() const { return id; }

    // Setters
    void set_data(const GLuint* data, GLuint count);
    void update_sub_data(const GLuint* data, GLuint offset, GLuint size);

    GLuint* map(BufferAccess access);
    void unmap() const;

    // Clear buffer data
    void clear();

private:
    BufferUsage usage;
    GLuint id;
    GLuint count;
};

}
