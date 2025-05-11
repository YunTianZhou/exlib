#include <iostream>

#include <exlib/window/window.hpp>
#include <exlib/graphics/image.hpp>

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Window Icon Test");

    if (!window.is_exist())
        return -1;

    // Set icon of the window
    ex::Image image(RES_DIR"settings.png");
    window.set_icon(image);

    // Main event loop
    while (window.is_open()) {
        window.clear(ex::Color::Magenta);
        window.display();
        window.poll_events();
    }

    // Destroy the window
    window.destroy();
}
