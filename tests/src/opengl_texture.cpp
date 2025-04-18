#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <exlib/window/window.hpp>
#include <exlib/opengl/vertex_buffer.hpp>
#include <exlib/opengl/index_buffer.hpp>
#include <exlib/opengl/vertex_array.hpp>
#include <exlib/opengl/shader.hpp>
#include <exlib/opengl/render.hpp>
#include <exlib/opengl/texture.hpp>

// Generate a simple checkerboard pattern
static std::vector<unsigned char> generate_checkerboard(int width, int height, int check_size) {
    std::vector<unsigned char> data(width * height * 4);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            bool even = ((x / check_size) + (y / check_size)) % 2 == 0;
            unsigned char color = even ? 255 : 0;
            int idx = (y * width + x) * 4;
            data[idx + 0] = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
            data[idx + 3] = 255;
        }
    }
    return data;
}

int main() {
    // Create window
    ex::Window& window = ex::Window::create({ 800, 600 }, "OpenGL Texture Test");
    if (!window.is_open()) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }
    window.set_display_interval(1);

    // Define quad vertices: position.xy, texcoord.xy
    float vertices[] = {
        // x,    y,    u,    v
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    // Create buffers and VAO
    ex::gl::VertexBuffer vbo(ex::gl::BufferUsage::Static, vertices, sizeof(vertices));
    ex::gl::IndexBuffer ibo(ex::gl::BufferUsage::Static, indices, 6);
    ex::gl::VertexArray vao;
    vao.bind();
    vao.set_layout(vbo, {
        {2, ex::gl::Type::Float, false}, // position
        {2, ex::gl::Type::Float, false}  // texcoord
        });
    vao.unbind();

    // Create checkerboard texture
    const int tex_w = 128, tex_h = 128, check_size = 16;
    auto checker = generate_checkerboard(tex_w, tex_h, check_size);
    ex::gl::Texture texture({ tex_w, tex_h }, checker.data());
    texture.set_filter(ex::gl::Texture::Filter::Nearest, ex::gl::Texture::Filter::Nearest);
    texture.set_wrap(ex::gl::Texture::Wrap::Repeat, ex::gl::Texture::Wrap::Repeat);
    texture.generate_mipmaps();

    // Shader sources
    const char* vs_source = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTex;
        out vec2 TexCoord;
        uniform mat4 uModel;
        void main() {
            TexCoord = aTex;
            gl_Position = uModel * vec4(aPos, 0.0, 1.0);
        }
    )";
    const char* fs_source = R"(
        #version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D uTexture;
        void main() {
            FragColor = texture(uTexture, TexCoord);
        }
    )";

    ex::gl::Shader::ProgramSource src{ vs_source, fs_source };
    ex::gl::Shader shader(src);

    // Transformation matrix
    float angle = 0.0f;

    // Main loop
    while (window.is_open()) {
        window.clear(ex::Color::Cyan);

        // Rotate quad slowly
        angle += 0.01f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });

        shader.bind();
        shader.set_uniform_matrix("uModel", model);

        texture.bind(0);
        shader.set_uniform_vec1("uTexture", 0);

        vao.bind();
        ex::gl::Render::draw_elements(ex::gl::PrimitiveType::Triangles, vao, ibo, shader);

        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}
