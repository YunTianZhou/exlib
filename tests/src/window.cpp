#include <iostream>

#include <exlib/window.hpp>

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

void on_iconify(int iconified) {
    if (iconified)
        std::cout << "Window minimized!" << std::endl;
    else
        std::cout << "Window restored!" << std::endl;
}

void on_maximize(int maximized) {
    if (maximized)
        std::cout << "Window maximized!" << std::endl;
    else
        std::cout << "Window restored!" << std::endl;
}

void on_focus(int focused) {
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
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Example Window");

    if (!window.is_exist())
        return -1;

    // Set callbacks
    ex::Window::Callback callbacks;
    callbacks.set_close(on_close);
    callbacks.set_size(on_size);
    callbacks.set_framebuffer_size(on_framebuffer_size);
    callbacks.set_position(on_position);
    callbacks.set_iconify(on_iconify);
    callbacks.set_maximize(on_maximize);
    callbacks.set_focus(on_focus);
    callbacks.set_refresh(on_refresh);

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
    callbacks.disable_close();
    callbacks.disable_size();

    // Main event loop
    while (window.is_open()) {
        window.poll_events();
        window.swap_buffers();
    }

    // Destroy the window
    window.destroy();
}
