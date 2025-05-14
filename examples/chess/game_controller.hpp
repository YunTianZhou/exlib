#pragma once
#include <exlib/exlib.hpp>
#include "game_model.hpp"

class GameController {
public:
    ex::Vec2d mpos;
    int midx;
    bool lbutton = false;
    bool rbutton = false;
    bool prev_lbutton = false;
    bool prev_rbutton = false;

    void update(ex::Window& window, GameModel& model);

private:
    void update_cursor(const WindowState& ws);
    void update_lbutton(GameModel& model);
    void update_rbutton(GameModel& model);
    void update_by_state(GameModel& model);
};
