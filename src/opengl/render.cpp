#include "exlib/opengl/render.hpp"
#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/vertex_buffer.hpp"
#include "exlib/opengl/index_buffer.hpp"
#include "exlib/opengl/shader.hpp"

namespace ex::gl {

void Render::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::set_clear_color(Color color) {
    glClearColor(
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    );
}

void Render::set_viewport(Vec2i pos, Vec2i size) {
    glViewport(pos.x, pos.y, size.x, size.y);
}

void Render::draw_arrays(Mode mode, const VertexArray& vao, const Shader& shader, GLint first, GLsizei count) {
    vao.bind();
    shader.bind();

    if (count == -1)
        count = vao.get_count();

    glDrawArrays((GLenum) (mode), first, count);
}

void Render::draw_elements(Mode mode, const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader, GLint first, GLsizei count) {
    vao.bind();
    ibo.bind();
    shader.bind();

    if (count == -1)
        count = ibo.get_count();

    glDrawElements((GLenum) (mode), count, GL_UNSIGNED_INT, (void*) (first * sizeof(GLuint)));
}

}