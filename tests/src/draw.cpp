#include <iostream>
#include <cmath>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 1200, 600 }, "Draw Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Set the center of the hexagon and the radius
    ex::Vec2f center{ 150.0f, 150.0f };
    float radius = 100.0f;

    // Define the vertices for the hexagon
    std::vector<ex::Vertex> vertices;

    // Add the center vertex
    vertices.push_back(ex::Vertex({ center.x, center.y }, { 255, 255, 255, 255 }));

    // Calculate and add the surrounding vertices for the hexagon
    for (int i = 0; i < 6; i++) {
        constexpr float PI = 3.1415927f;
        float angle = i * 2.0f * PI / 6.0f; // 360 degrees / 6 sides
        ex::Vec2f vertex = {
            center.x + radius * cos(angle),
            center.y + radius * sin(angle)
        };
        // Use a color that varies depending on the vertex position (for a colorful effect)
        ex::Color color = { (uint8_t) (255 * cos(angle)), (uint8_t) (255 * sin(angle)), 128, 255 };
        vertices.push_back(ex::Vertex(vertex, color));
    }
    vertices.push_back(vertices[1]);

    // Set swapping interval
    window.set_display_interval(1);

    while (window.is_open()) {
        // Clear the screen
        window.clear();

        // Draw the hexagon
        ex::Draw::State state(ex::PrimitiveType::TriangleFan);
        ex::Draw::draw(vertices, state);

        // Swap buffers and poll events
        window.display();
        window.poll_events();
    }

    // Destroy the window
    window.destroy();
    return 0;
}
