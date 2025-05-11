#include <iostream>
#include <cmath>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/rect_shape.hpp>
#include <exlib/graphics/texture.hpp>

int main() {
    // 1) Create a window
    ex::Window& window = ex::Window::create({ 1200, 600 }, "Rect Shape Texture Test");
    if (!window.is_exist()) {
        std::cerr << "Failed to create window!\n";
        return -1;
    }
    window.set_display_interval(1);

    // 2) Load a texture from file
    ex::Texture texture(RES_DIR"brick.png");

    // 3) Generate mipmaps and set filtering / wrapping
    texture.set_filter(
        ex::Texture::Filter::LinearMipmapLinear,
        ex::Texture::Filter::Linear
    );
    texture.set_wrap(
        ex::Texture::Wrap::Repeat,
        ex::Texture::Wrap::Repeat
    );
    texture.generate_mipmaps();

    // 4) Prepare two RectShapes
    //    Left quad: single‑mapped
    ex::RectShape quad1({ 300.0f, 300.0f });
    quad1.set_texture(&texture, true);     // reset UV to full texture
    quad1.set_origin(quad1.get_geometric_center());
    quad1.set_position({ 300.0f, 300.0f });

    //    Right quad: tiled 4×4
    ex::RectShape quad2({ 300.0f, 300.0f });
    quad2.set_texture(&texture, true);
    // Extend UV so it repeats 4× in each axis
    auto ts = texture.get_size();
    quad2.set_texture_rect({ { 0, 0 }, { ts.x * 4, ts.y * 4 } });
    quad2.set_origin(quad2.get_geometric_center());
    quad2.set_position({ 900.0f, 300.0f });

    // 5) Main loop: rotate both quads in opposite directions
    float angle = 0.0f;
    while (window.is_open()) {
        window.clear();

        angle += 0.5f;
        quad1.set_rotation(angle);
        quad2.set_rotation(-angle);

        ex::Draw::draw(quad1);
        ex::Draw::draw(quad2);

        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}