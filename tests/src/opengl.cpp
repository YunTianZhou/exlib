#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <exlib/window/window.hpp>
#include <exlib/opengl/index_buffer.hpp>
#include <exlib/opengl/vertex_buffer.hpp>
#include <exlib/opengl/vertex_array.hpp>
#include <exlib/opengl/shader.hpp>
#include <exlib/opengl/render.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "OpenGL Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Define triangle vertices (x, y, z)
    GLfloat vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // Define indices for triangle
    GLuint indices[] = {
        0, 1, 2
    };

    // Create Vertex Buffer
    ex::gl::VertexBuffer vbo(ex::gl::BufferUsage::Static, vertices, sizeof(vertices));

    // Create Index Buffer
    ex::gl::IndexBuffer ibo(ex::gl::BufferUsage::Static, indices, 3);

    // Create Vertex Array and configure layout
    ex::gl::VertexArray vao;
    vao.bind();
    vao.add_buffer(vbo, { { 3, ex::gl::Type::Float, false } });
    vao.unbind();

    // Shader source
    std::string vertex_shader = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform mat4 uModel;

        void main() {
            gl_Position = uModel * vec4(aPos, 1.0);
        }
    )";

    std::string fragment_shader = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.5, 0.2, 0.8, 1.0);
        }
    )";

    // Create Shader
    ex::gl::Shader::ProgramSource source{ vertex_shader, fragment_shader };
    ex::gl::Shader shader(source);

    // Set swapping interval
    window.set_swap_interval(1);

    // Main event loop
    float angle = 0.0f;  // Rotation angle
    while (window.is_open()) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Increase rotation angle over time
        angle += 0.01f;

        // Create rotation model matrix (rotate around Z-axis)
        glm::mat4 model = glm::mat4(1.0f);  // Identity matrix
        model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate around Z-axis

        // Bind shader and pass the model matrix
        shader.bind();
        shader.set_uniform_matrix("uModel", model);

        // Draw the triangle
        ex::gl::Render::draw_elements(ex::gl::Render::Mode::Triangles, vao, ibo, shader);

        // Swap buffers and poll events
        window.swap_buffers();
        window.poll_events();
    }

    // Destroy the window
    window.destroy();
    return 0;
}
