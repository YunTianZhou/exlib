#pragma once
#include <exlib/exlib.hpp>
#include "game_model.hpp"

struct GameRenderer {
public:
    GameRenderer();

    void draw(const GameModel& model);
    
private:
    void draw_board(const GameModel& model);
    void draw_pieces(const GameModel& model);
    void draw_highlights(const GameModel& model);
    void draw_hints(const GameModel& model);
    void draw_marks_highlights(const GameModel& model);
    void draw_marks_arrows(const GameModel& model);
    void draw_promotion_menu(const GameModel& model);

private:
    void highlight(const WindowState& ws, int idx, ex::Color color, bool fill = true);
    void draw_arrow(const WindowState& ws, ex::Vec2f from, ex::Vec2f to, bool lshape = false);

private:
    ex::Texture pieces;
    ex::Font arial;
    ex::RectShape top;
    bool has_top = false;
};
