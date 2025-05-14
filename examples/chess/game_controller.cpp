#include "game_controller.hpp"
#include "game_renderer.hpp"

using namespace ex;

using Mode = MovementState::Mode;

void GameController::update(Window& window, GameModel& model) {
    model.ws.update(window);
    update_cursor(model.ws);
    update_lbutton(model);
    update_rbutton(model);
    update_by_state(model);
}

void GameController::update_cursor(const WindowState& ws) {
    mpos = Cursor::get_position();
    Vec2f rel{ float(mpos.x) - ws.offset.x, float(mpos.y) - ws.offset.y };
    int col = int(rel.x / ws.cell_size);
    int row = int(rel.y / ws.cell_size);
    midx = (row >= 0 && row < 8 && col >= 0 && col < 8) ? (row * 8 + col) : -1;

    prev_lbutton = lbutton;
    prev_rbutton = rbutton;
    lbutton = Cursor::is_button_pressed(Cursor::Button::Left);
    rbutton = Cursor::is_button_pressed(Cursor::Button::Right);
}


void GameController::update_lbutton(GameModel& model) {
    if (lbutton == prev_lbutton) return;

    MovementState& mm = model.mm;
    MarkingState& mk = model.mk;

    // Clear marking state
    mk.reset();

    if (mm.mode == Mode::Promottion) {
        if (lbutton && midx != -1) {
            // Click when promoting
            const char* options = model.board.is_white_to_move() ? "QRBN" : "qrbn";
            int promote_idx = mm.move.to;
            int dir = model.board.is_white_to_move() ? 1 : -1;

            for (int i = 0; i < 4; i++) {
                int curr_idx = promote_idx + dir * i * 8;

                if (midx == curr_idx) {
                    model.promote(options[i]);
                    return;
                }
            }

            mm.reset();
        }
        return;
    }

    if (lbutton && (mm.mode == Mode::None || mm.mode == Mode::Selected)) {
        // Click a square
        if (midx != -1) {
            char piece = model.board[midx];
            bool is_mine = (piece != '.') && (is_piece_white(piece) == model.board.is_white_to_move());
            if (is_mine) {
                mm.move = { midx, -1 };
                mm.mode = Mode::Dragging;
            }
            else if (mm.mode == Mode::Selected) {
                Move mv{ mm.move.from, midx };
                if (model.is_legal(mv)) {
                    if (model.board.is_promotion_like(mv)) {
                        model.play(mv);
                    }
                    else {
                        mm.move = mv;
                        model.last_move = mv;
                        mm.is_castle = (std::tolower(model.board[mv.from]) == 'k' &&
                            std::abs(mv.from - mv.to) == 2);
                        mm.start_slide();
                    }
                }
                else {
                    mm.reset();
                }
            }
        }
    }
    else if (!lbutton && mm.mode == Mode::Dragging) {
        // Release dragging
        Move mv{ mm.move.from, midx };
        if (midx == -1 || !model.is_legal(mv)) {
            mm.mode = Mode::Selected;
            mm.move.to = -1;
        }
        else {
            model.play(mv);
        }
    }

}

void GameController::update_rbutton(GameModel& model) {
    if (rbutton == prev_rbutton) return;

    MovementState& mm = model.mm;
    MarkingState& mk = model.mk;

    Mode mode = mm.mode;

    if (mode == Mode::Sliding) return;

    // Clear movement state
    if (mode != Mode::None)
        mm.reset();

    if (mode == Mode::Dragging || mode == Mode::Promottion) return;

    if (rbutton && !mk.is_marking) {
        // Start marking
        if (midx != -1) {
            mk.start_marking(midx);
        }
    }
    else if (!rbutton && mk.is_marking) {
        // Stop marking
        mk.end_marking(midx);
    }
}

void GameController::update_by_state(GameModel& model) {
    MovementState& mm = model.mm;

    if (mm.mode == Mode::Dragging) {
        mm.move.to = midx;
    }
    else if (mm.mode == Mode::Sliding) {
        mm.advance_slide();
        if (mm.slide_pct >= 1.0f) {
            model.play(mm.move);
            mm.reset();
        }
    }
}
