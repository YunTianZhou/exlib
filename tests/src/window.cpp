#include <iostream>

#include <exlib/window/window.hpp>

void on_close() {
    std::cout << "Window closed!" << std::endl;
}

void on_size(ex::Vec2i size) {
    std::cout << "Window resized to: " << size.x << "x" << size.y << std::endl;
}

void on_framebuffer_size(ex::Vec2i size) {
    std::cout << "Framebuffer resized to: " << size.x << "x" << size.y << std::endl;
}

void on_position(ex::Vec2i pos) {
    std::cout << "Window moved to: (" << pos.x << ", " << pos.y << ")" << std::endl;
}

void on_iconify(bool iconified) {
    if (iconified)
        std::cout << "Window minimized!" << std::endl;
    else
        std::cout << "Window restored!" << std::endl;
}

void on_maximize(bool maximized) {
    if (maximized)
        std::cout << "Window maximized!" << std::endl;
    else
        std::cout << "Window restored!" << std::endl;
}

void on_focus(bool focused) {
    if (focused)
        std::cout << "Window gained focus!" << std::endl;
    else
        std::cout << "Window lost focus!" << std::endl;
}

void on_refresh() {
    std::cout << "Window refreshed!" << std::endl;
}

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Window Test");

    if (!window.is_exist())
        return -1;

    // Set callbacks
    window.set_close_callback(on_close);
    window.set_size_callback(on_size);
    window.set_framebuffer_size_callback(on_framebuffer_size);
    window.set_position_callback(on_position);
    window.set_iconify_callback(on_iconify);
    window.set_maximize_callback(on_maximize);
    window.set_focus_callback(on_focus);
    window.set_refresh_callback(on_refresh);

    // Test basic window functions
    window.set_title("Testing Window Features");
    std::cout << "Title: " << window.get_title() << std::endl;

    window.set_size({ 1024, 768 });
    ex::Vec2i size = window.get_size();
    std::cout << "Window size: " << size.x << "x" << size.y << std::endl;

    window.set_position({ 200, 150 });
    ex::Vec2i pos = window.get_position();
    std::cout << "Window position: (" << pos.x << ", " << pos.y << ")" << std::endl;

    window.set_aspect_ratio(16, 9);
    std::cout << "Aspect ratio set to 16:9" << std::endl;

    window.set_opacity(0.8f);
    std::cout << "Opacity set to 0.8" << std::endl;

    window.iconify();
    window.restore();
    window.maximize();

    // Disable some callbacks
    window.disable_close_callback();
    window.disable_size_callback();

    // Main event loop
    while (window.is_open()) {
        window.poll_events();
        window.swap_buffers();
    }

    // Destroy the window
    window.destroy();
}
