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
    if (!state.texture)
        draw_color(vertices.data(), (int) vertices.size(), state);
    else
        draw_texture(vertices.data(), (int) vertices.size(), state);
}

void Draw::draw(const Vertex* start, int count, const State& state) {
    if (!state.texture)
        draw_color(start, count, state);
    else
        draw_texture(start, count, state);
}

void Draw::draw(const Drawable& drawable) {
    drawable.draw();
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
            {2, gl::Type::Float, false},  // texcoord
            {4, gl::Type::Float, false}   // color
        });
    }
    if (!texture_shader) {
        gl::Shader::ProgramSource src = {
        // Vertex shader
        R"(
        #version 330 core
        layout(location = 0) in vec2 a_position;
        layout(location = 1) in vec2 a_texcoord;
        layout(location = 2) in vec4 a_color;
        uniform mat4 u_transform;
        uniform vec2 u_texRecip;
        out vec2 v_texcoord;
        out vec4 v_color;
        void main() {
            gl_Position = u_transform * vec4(a_position, 0.0, 1.0);
            v_texcoord = a_texcoord * u_texRecip;
            v_color    = a_color;
        }
        )",
        // Fragment shader
        R"(
        #version 330 core
        in vec2 v_texcoord;
        in vec4 v_color;
        uniform sampler2D u_texture;
        out vec4 fragColor;
        void main() {
            vec4 tex = texture(u_texture, v_texcoord);
            fragColor = tex * v_color;
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

void Draw::draw_color(const Vertex* start, int count, const State& state) {
    init_color_pipeline();
    if (count <= 0) return;

    std::vector<GLfloat> data;
    data.reserve(count * 6);
    while (count--) {
        data.push_back(start->pos.x);
        data.push_back(start->pos.y);
        data.push_back(start->color.r / 255.0f);
        data.push_back(start->color.g / 255.0f);
        data.push_back(start->color.b / 255.0f);
        data.push_back(start->color.a / 255.0f);
        start++;
    }
    color_vbo->set_data(data.data(), GLsizei(data.size() * sizeof(GLfloat)));

    color_shader->set_uniform_matrix("u_transform", get_ortho_transform(state.transform));
    
    gl::Render::draw_arrays(state.type, *color_vao, *color_shader);
}

void Draw::draw_texture(const Vertex* start, int count, const State& state) {
    init_texture_pipeline();
    if (count <= 0) return;

    Vec2f tex_size(state.texture->get_size());
    texture_shader->set_uniform_vec2("u_texRecip", 1.0f / tex_size.x, 1.0f / tex_size.y);

    std::vector<GLfloat> data;
    data.reserve(count * 8);
    while (count--) {
        data.push_back(start->pos.x);
        data.push_back(start->pos.y);
        data.push_back(start->tex_coords.x);
        data.push_back(start->tex_coords.y);
        data.push_back(start->color.r / 255.0f);
        data.push_back(start->color.g / 255.0f);
        data.push_back(start->color.b / 255.0f);
        data.push_back(start->color.a / 255.0f);
        start++;
    }
    texture_vbo->set_data(data.data(), GLsizei(data.size() * sizeof(GLfloat)));

    texture_shader->set_uniform_matrix("u_transform", get_ortho_transform(state.transform));
    texture_shader->set_uniform_vec1("u_texture", 0);

    state.texture->bind(0);
    gl::Render::draw_arrays(state.type, *texture_vao, *texture_shader);
}

}
