#include <glm/gtc/matrix_transform.hpp>

#include "exlib/graphics/draw.hpp"
#include "exlib/opengl/vertex_buffer.hpp"
#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/shader.hpp"
#include "exlib/opengl/render.hpp"
#include "exlib/window/window.hpp"
#include "exlib/graphics/texture.hpp"

namespace ex {

std::unique_ptr<gl::Shader>         Draw::color_shader = nullptr;
std::unique_ptr<gl::VertexBuffer>   Draw::color_vbo = nullptr;
std::unique_ptr<gl::VertexArray>    Draw::color_vao = nullptr;

std::unique_ptr<gl::Shader>         Draw::texture_shader = nullptr;
std::unique_ptr<gl::VertexBuffer>   Draw::texture_vbo = nullptr;
std::unique_ptr<gl::VertexArray>    Draw::texture_vao = nullptr;

void Draw::draw(const std::vector<Vertex>& vertices, const State& state) {
    if (state.texture == nullptr)
        draw_color(vertices, state);
    else
        draw_texture(vertices, state);
}

void Draw::draw(const Drawable& drawable) {
    drawable.draw();
}

void Draw::draw(const std::vector<Drawable>& drawables) {
    for (const auto& d : drawables) {
        d.draw();
    }
}

void Draw::init_color_pipeline() {
    if (!color_vbo) {
        color_vbo = std::make_unique<gl::VertexBuffer>(gl::BufferUsage::Dynamic);
    }
    if (!color_vao) {
        color_vao = std::make_unique<gl::VertexArray>();
        color_vao->set_layout(*color_vbo, {
            {2, gl::Type::Float, false},  // position
            {4, gl::Type::Float, false}   // color
        });
    }
    if (!color_shader) {
        gl::Shader::ProgramSource src = {
            // Vertex shader
            R"(
            #version 330 core
            layout(location = 0) in vec2 a_position;
            layout(location = 1) in vec4 a_color;
            uniform mat4 u_transform;
            out vec4 v_color;
            void main() {
                gl_Position = u_transform * vec4(a_position, 0.0, 1.0);
                v_color = a_color;
            }
            )",
            // Fragment shader
            R"(
            #version 330 core
            in vec4 v_color;
            out vec4 fragColor;
            void main() {
                fragColor = v_color;
            }
            )"
        };
        color_shader = std::make_unique<gl::Shader>(src);
    }
}

// Initialize texture pipeline
void Draw::init_texture_pipeline() {
    if (!texture_vbo) {
        texture_vbo = std::make_unique<gl::VertexBuffer>(gl::BufferUsage::Dynamic);
    }
    if (!texture_vao) {
        texture_vao = std::make_unique<gl::VertexArray>();
        texture_vao->set_layout(*texture_vbo, {
            {2, gl::Type::Float, false},  // position
            {2, gl::Type::Float, false}   // texcoords
        });
    }
    if (!texture_shader) {
        gl::Shader::ProgramSource src = {
            // Vertex shader
            R"(
            #version 330 core
            layout(location = 0) in vec2 a_position;
            layout(location = 1) in vec2 a_texcoord;
            uniform mat4 u_transform;
            out vec2 v_texcoord;
            void main() {
                gl_Position = u_transform * vec4(a_position, 0.0, 1.0);
                v_texcoord = a_texcoord;
            }
            )",
            // Fragment shader
            R"(
            #version 330 core
            in vec2 v_texcoord;
            uniform sampler2D u_texture;
            out vec4 fragColor;
            void main() {
                fragColor = texture(u_texture, v_texcoord);
            }
            )"
        };
        texture_shader = std::make_unique<gl::Shader>(src);
    }
}

glm::mat4 Draw::get_ortho_transform(const glm::mat4* _transform) {
    glm::mat4 transform = _transform ? *_transform : glm::mat4(1.0f);
    Vec2f fb = (Vec2f) Window::get_instance().get_framebuffer_size();
    glm::mat4 ortho = glm::ortho(0.0f, fb.x, fb.y, 0.0f, -1.0f, 1.0f);
    return ortho * transform;
}

void Draw::draw_color(const std::vector<Vertex>& vertices, const State& state) {
    init_color_pipeline();
    if (vertices.size() < 3) return;

    std::vector<GLfloat> data;
    data.reserve(vertices.size() * 6);
    for (auto& v : vertices) {
        data.push_back(v.pos.x);
        data.push_back(v.pos.y);
        data.push_back(v.color.r / 255.0f);
        data.push_back(v.color.g / 255.0f);
        data.push_back(v.color.b / 255.0f);
        data.push_back(v.color.a / 255.0f);
    }
    color_vbo->set_data(data.data(), GLsizei(data.size() * sizeof(GLfloat)));

    color_shader->set_uniform_matrix("u_transform", get_ortho_transform(state.transform));

    gl::Render::draw_arrays(state.type, *color_vao, *color_shader);
}

void Draw::draw_texture(const std::vector<Vertex>& vertices, const State& state) {
    init_texture_pipeline();
    if (vertices.size() < 3) return;

    std::vector<GLfloat> data;
    data.reserve(vertices.size() * 4);
    Vec2f texture_size(state.texture->get_size());
    for (auto& v : vertices) {
        data.push_back(v.pos.x);
        data.push_back(v.pos.y);
        data.push_back(v.tex_coords.x / texture_size.x);
        data.push_back(v.tex_coords.y / texture_size.y);
    }
    texture_vbo->set_data(data.data(), GLsizei(data.size() * sizeof(GLfloat)));

    texture_shader->set_uniform_matrix("u_transform", get_ortho_transform(state.transform));
    texture_shader->set_uniform_vec1("u_texture", 0);

    state.texture->bind(0);
    gl::Render::draw_arrays(state.type, *texture_vao, *texture_shader);
}

}
