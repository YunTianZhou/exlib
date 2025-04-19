#include <iostream>
#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/sprite.hpp>
#include <exlib/graphics/texture.hpp>

int main() {
    // Create window
    ex::Window& window = ex::Window::create({ 800, 600 }, "Sprite Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Load texture
    ex::Texture texture;
    if (!texture.load_from_file(IMG_DIR"github.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }

    // Create sprite
    ex::Sprite sprite(texture);
    ex::Vec2f center = sprite.get_bounds().get_center();
    sprite.scale({ 0.1f, 0.1f });
    sprite.set_position(-center + (ex::Vec2f(window.get_size()) / 2.0f));
    sprite.set_origin(center);

    // Display settings
    window.set_display_interval(1);

    while (window.is_open()) {
        window.clear(ex::Color::White);

        // Animate sprite
        sprite.rotate(1.0f);
        sprite.scale({ 1.001f, 1.001f });

        // Draw sprite
        ex::Draw::draw(sprite);

        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}
