#include "exlib/opengl/render.hpp"
#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/vertex_buffer.hpp"
#include "exlib/opengl/index_buffer.hpp"
#include "exlib/opengl/shader.hpp"

namespace ex::gl {

void Render::draw_arrays(PrimitiveType type, const VertexArray& vao, const Shader& shader, GLint first, GLsizei size) {
    vao.bind();
    shader.bind();

    if (size == -1)
        size = vao.get_bound_buffer()->get_size();

    glDrawArrays((GLenum) (type), first, size);
}

void Render::draw_elements(PrimitiveType type, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLint first, GLsizei count) {
    vao.bind();
    ibo.bind();
    shader.bind();

    if (count == -1)
        count = ibo.get_count();

    glDrawElements((GLenum) (type), count, GL_UNSIGNED_INT, (void*) (first * sizeof(GLuint)));
}

}