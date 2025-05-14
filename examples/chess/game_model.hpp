#pragma once
#include <set>
#include <exlib/exlib.hpp>
#include "board.hpp"

struct WindowState {
    ex::Vec2i win_size;
    float cell_size = 0;
    float board_size = 0;
    ex::Vec2f offset;

    void update(ex::Window& window);

};

struct MovementState {
    enum class Mode { None, Selected, Dragging, Sliding, Promottion } mode = Mode::None;
    Move move = null_move;
    bool is_castle = false;
    float slide_pct = 0.0f;

    void start_slide();
    void advance_slide();
    void reset();
};

struct MarkingState {
    bool is_marking = false;
    int start_pos = -1;
    std::set<Move> mark_buffer;

    void start_marking(int pos);
    void end_marking(int end_pos);
    void reset();
};

struct GameModel {
    WindowState ws;
    MovementState mm;
    MarkingState mk;

    Board board;
    Move last_move = null_move;
    std::array<std::vector<int>, 64> legal_moves;

    GameModel();
    bool is_legal(Move m) const;
    void play(Move m);
    void promote(char piece);

private:
    void apply_move(Move m);
    void rebuild_legal_moves();
};
