#include <iostream>
#include <cmath>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/circle_shape.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 1200, 600 }, "Circle Shape Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Create the circle shape
    ex::CircleShape circle(100.0f);  // radius 100
    circle.set_fill_color(ex::Color::White);
    circle.set_outline_color(ex::Color::Red);
    circle.set_outline_thickness(-3.0f);
    circle.set_origin(circle.get_geometric_center());

    // Set display refresh interval
    window.set_display_interval(1);

    while (window.is_open()) {
        // Clear the screen
        window.clear();

        // Animate the circle
        circle.rotate(1.0f);
        circle.move({ 0.5f, 0.5f });
        circle.scale({ 1.0005f, 1.0005f });

        // Draw the circle
        ex::Draw::draw(circle);

        // Update window
        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}
