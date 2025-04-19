#include "exlib/opengl/render.hpp"
#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/vertex_buffer.hpp"
#include "exlib/opengl/index_buffer.hpp"
#include "exlib/opengl/shader.hpp"

namespace ex::gl {

void Render::draw_arrays(PrimitiveType type, const VertexArray& vao, const Shader& shader, GLint first, GLsizei count) {
    vao.bind();
    shader.bind();

    if (count < 0)
        count = vao.get_count();

    glDrawArrays((GLenum) (type), first, count);
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