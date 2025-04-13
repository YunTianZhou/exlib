#include <glm/gtc/matrix_transform.hpp>

#include "exlib/graphics/draw.hpp"
#include "exlib/opengl/vertex_buffer.hpp"
#include "exlib/opengl/vertex_array.hpp"
#include "exlib/opengl/shader.hpp"
#include "exlib/opengl/render.hpp"
#include "exlib/window/window.hpp"

namespace ex {

std::unique_ptr<gl::Shader> Draw::color_shader = nullptr;
std::unique_ptr<gl::VertexBuffer> Draw::color_vbo = nullptr;
std::unique_ptr<gl::VertexArray> Draw::color_vao = nullptr;

std::unique_ptr<gl::Shader> Draw::texture_shader = nullptr;
std::unique_ptr<gl::VertexBuffer> Draw::texture_vbo = nullptr;
std::unique_ptr<gl::VertexArray> Draw::texture_vao = nullptr;

void Draw::draw(const std::vector<Vertex>& vertices, const State& state) {
    draw_color(vertices, state);
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
        std::vector<gl::VertexArray::Element> layout = {
            { 2, gl::Type::Float, false },         // Position: 2 floats (Vec2f)
            { 4, gl::Type::Float, false }          // Color: 4 floats
        };

        color_vao = std::make_unique<gl::VertexArray>();
        color_vao->set_layout(*color_vbo, layout);
    }

    if (!color_shader) {
        gl::Shader::ProgramSource source = {
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

        color_shader = std::make_unique<gl::Shader>(source);
    }
}

void Draw::init_texture_pipeline() {

}

void Draw::draw_color(const std::vector<Vertex>& vertices, const State& state) {
    init_color_pipeline();

    int size = (int) (vertices.size());
    if (size < 3) {
        return;
    }

    std::vector<GLfloat> data;
    data.reserve(vertices.size() * (2 + 4)); // 2 for pos, 4 for color

    for (const auto& vertex : vertices) {
        data.push_back(vertex.pos.x);
        data.push_back(vertex.pos.y);

        data.push_back(vertex.color.r / 255.0f);
        data.push_back(vertex.color.g / 255.0f);
        data.push_back(vertex.color.b / 255.0f);
        data.push_back(vertex.color.a / 255.0f);
    }

    color_vbo->set_data(data.data(), (GLsizei) data.size() * sizeof(GLfloat));

    glm::mat4 transform = state.transform == nullptr ? glm::mat4(1.0f) : *state.transform;
    Vec2f fb_size = (Vec2f) Window::get_instance().get_framebuffer_size();
    transform = glm::ortho(0.0f, fb_size.x, fb_size.y, 0.0f, -1.0f, 1.0f) * transform;

    color_shader->set_uniform_matrix("u_transform", transform);
    gl::Render::draw_arrays(state.type, *color_vao, *color_shader);
}

}
