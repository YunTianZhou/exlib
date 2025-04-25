#include <iostream>
#include <cmath>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/conv_shape.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create({ 800, 600 }, "Conv Shape Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Create a ConvShape (pentagon)
    ex::ConvShape convex;
    convex.set_point_count(5);

    convex.set_point(0, { 0.0f, 0.0f });
    convex.set_point(1, { 150.0f, 10.0f });
    convex.set_point(2, { 120.0f, 90.0f });
    convex.set_point(3, { 30.0f, 100.0f });
    convex.set_point(4, { 0.0f, 50.0f });

    convex.set_fill_color(ex::Color::Green);
    convex.set_outline_color(ex::Color::Red);
    convex.set_outline_thickness(-2.0f);
    convex.set_origin(convex.get_geometric_center());

    // Set display interval
    window.set_display_interval(1);

    // Main loop
    while (window.is_open()) {
        window.clear();

        convex.move({ 1.0f, 1.0f });
        convex.rotate(1.0f);
        ex::Draw::draw(convex);

        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}
