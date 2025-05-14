#include <exlib/exlib.hpp>
#include "game_model.hpp"
#include "game_renderer.hpp"
#include "game_controller.hpp"

using namespace ex;

int main() {
    Window& window = Window::create({ 800,800 }, "Chess");
    window.set_aspect_ratio(1, 1);
    window.set_display_interval(1);

    GameModel      model;
    GameRenderer   view;
    GameController ctrl;

    while (window.is_open()) {
        window.clear();
        ctrl.update(window, model);
        view.draw(model);
        window.display();
        window.poll_events();
    }

    return 0;

}
