#include "game_model.hpp"

using namespace ex;

using Mode = MovementState::Mode;

void WindowState::update(Window& window) {
    win_size = window.get_size();
    cell_size = std::min(win_size.x, win_size.y) / 8.0f;
    board_size = cell_size * 8;
    offset = { (win_size.x - board_size) / 2, (win_size.y - board_size) / 2 };
}

void MovementState::start_slide() {
    mode = Mode::Sliding;
    slide_pct = 0.0f;
}

void MovementState::advance_slide() {
    slide_pct = std::min(slide_pct + 0.1f, 1.0f);
}

void MovementState::reset() {
    mode = Mode::None;
    move = null_move;
    is_castle = false;
    slide_pct = 0.0f;
}

void MarkingState::start_marking(int pos) {
    is_marking = true;
    start_pos = pos;
}

void MarkingState::end_marking(int end_pos) {
    Move mark = { start_pos, end_pos };

    is_marking = false;
    start_pos = -1;

    if (end_pos == -1)
        return;  // marking canceled

    if (mark_buffer.find(mark) == mark_buffer.end())
        mark_buffer.insert(mark);
    else
        mark_buffer.erase(mark);
}

void MarkingState::reset() {
    is_marking = false;
    start_pos = -1;
    mark_buffer.clear();
}

GameModel::GameModel() {
    rebuild_legal_moves();
}

bool GameModel::is_legal(Move m) const {
    if (m.from < 0 && m.from >= 64) return false;
    const auto& targets = legal_moves[m.from];
    return std::find(targets.begin(), targets.end(), m.to) != targets.end();
}

void GameModel::play(Move m) {
    if (!is_legal(m)) return;
    if (board.is_promotion_like(m)) {
        mm.mode = Mode::Promottion;
        mm.move = m;
    }
    else {
        apply_move(m);
    }
}

void GameModel::promote(char piece) {
    if (mm.mode != Mode::Promottion) return;
    mm.move.promotion = piece;
    apply_move(mm.move);
}

void GameModel::apply_move(Move m) {
    board.make_move(m);
    last_move = m;
    rebuild_legal_moves();
    mm.reset();
}

void GameModel::rebuild_legal_moves() {
    for (auto& v : legal_moves) v.clear();
    for (auto m : board.gen_legal_moves(board.is_white_to_move())) {
        if (m.promotion == 0 || std::tolower(m.promotion) == 'q') {
            legal_moves[m.from].push_back(m.to);
        }
    }
}
