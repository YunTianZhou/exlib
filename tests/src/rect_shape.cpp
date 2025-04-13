#include <iostream>
#include <cmath>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/rect_shape.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 1200, 600 }, "Rect Shape Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Create the rect shape
    ex::RectShape rect({ 100.0f, 100.0f });
    rect.set_fill_color(ex::Color::White);
    rect.set_outline_color(ex::Color::Blue);
    rect.set_outline_thickness(-3.0f);
    rect.set_origin(rect.get_geometric_center());

    // Set swapping interval
    window.set_display_interval(1);

    while (window.is_open()) {
        // Clear the screen
        window.clear();

        // Draw the rect
        rect.rotate(1.0f);
        rect.move({ 1.0f, 1.0f });
        rect.scale({ 1.001f, 1.001f });
        ex::Draw::draw(rect);

        // Swap buffers and poll events
        window.display();
        window.poll_events();
    }

    // Destroy the window
    window.destroy();
    return 0;
}
