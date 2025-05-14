#include <cmath>
#include "game_renderer.hpp"
#include "game_controller.hpp"
#include "res/pieces_image.hpp"
#include "res/arial_font.hpp"

using namespace ex;

using Mode = MovementState::Mode;

GameRenderer::GameRenderer() {
    if (!pieces.load_from_memory(pieces_image, pieces_image_size)) {
        throw std::runtime_error("Failed to load pieces texture");
    }
    if (!arial.open_from_memory(arial_font, arial_font_size)) {
        throw std::runtime_error("Failed to load arial font");
    }
}

static Vec2f cell_world_pos(int idx, const WindowState& ws) {
    int r = idx / 8;
    int c = idx % 8;
    return { c * ws.cell_size + ws.offset.x, r * ws.cell_size + ws.offset.y };
}

void GameRenderer::draw(const GameModel& model) {
    draw_board(model);
    draw_highlights(model);

    Mode mode = model.mm.mode;
    if (mode == Mode::Dragging || mode == Mode::Selected || mode == Mode::Promottion) {
        // Moving pieces
        draw_pieces(model);
        draw_hints(model);
        if (has_top) Draw::draw(top); // draw top piece after hints
        if (mode == Mode::Promottion)
            draw_promotion_menu(model);
    } 
    else {
        draw_marks_highlights(model);
        draw_pieces(model);
        if (has_top) Draw::draw(top); // draw top piece for sliding
        draw_marks_arrows(model);
    }
}

void GameRenderer::draw_board(const GameModel& model) {
    const WindowState& ws = model.ws;

    RectShape cell({ ws.cell_size, ws.cell_size });
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cell.set_position({
                j * ws.cell_size + ws.offset.x,
                i * ws.cell_size + ws.offset.y
            });
            cell.set_fill_color(((i + j) % 2 == 0)
                ? Color{ 237, 214, 176, 255 }
                : Color{ 184, 135, 98, 255 });
            Draw::draw(cell);
        }
    }

    Vec2f spacing = { ws.cell_size / 15, ws.cell_size / 25 };
    int char_size = int(ws.cell_size * 0.225);
    for (int x = 0; x < 8; x++) {
        ex::Text text(arial, U"", char_size);

        // Letter
        text.set_string(std::u32string(1, U'a' + x));
        text.set_position({
            (x + 1) * ws.cell_size - spacing.x * 1.5f - text.get_bounds().size.x + ws.offset.x,
            ws.cell_size * 8 - spacing.y * 2 - char_size + ws.offset.y
            });
        text.set_fill_color((x % 2 == 0)
            ? Color{ 237, 214, 176, 255 }
            : Color{ 184, 135, 98, 255 });
        Draw::draw(text);

        // Number
        text.set_string(std::u32string(1, U'0' + (8 - x)));
        text.set_position({
            spacing.x + ws.offset.x,
            spacing.y + x * ws.cell_size + ws.offset.y
        });
        text.set_fill_color((x % 2 == 1)
            ? Color{ 237, 214, 176, 255 }
            : Color{ 184, 135, 98, 255 });
        Draw::draw(text);
    }
}

void GameRenderer::draw_highlights(const GameModel& model) {
    highlight(model.ws, model.last_move.from, { 203, 176, 0, 128 });
    highlight(model.ws, model.last_move.to,   { 203, 176, 0, 128 });

    Mode mode = model.mm.mode;
    if (mode == Mode::Dragging || mode == Mode::Selected)
        highlight(model.ws, model.mm.move.from, { 203, 176, 0, 128 });

    if (mode == Mode::Dragging)
        highlight(model.ws, model.mm.move.to, { 255, 255, 255, 200 }, false);
}

void GameRenderer::draw_hints(const GameModel& model) {
    CircleShape dot(model.ws.cell_size / 6);
    dot.set_fill_color({ 0, 0, 0, 32 });
    CircleShape ring(model.ws.cell_size / 2);
    ring.set_outline_color({ 0, 0, 0, 32 }); ring.set_outline_thickness(-8.0f);
    ring.set_fill_color(Color::Transparent);

    for (int tgt : model.legal_moves[model.mm.move.from]) {
        Vec2f p = cell_world_pos(tgt, model.ws);
        if (model.board[tgt] == '.') {
            p += {model.ws.cell_size / 3, model.ws.cell_size / 3};
            dot.set_position(p);
            Draw::draw(dot);
        }
        else {
            ring.set_position(p);
            Draw::draw(ring);
        }
    }
}

void GameRenderer::draw_marks_highlights(const GameModel& model) {
    const WindowState& ws = model.ws;

    for (Move mark : model.mk.mark_buffer) {
        Vec2f from = cell_world_pos(mark.from, ws);
        Vec2f to = cell_world_pos(mark.to, ws);

        if (from == to) {
            // Highligh square
            highlight(model.ws, mark.from, { 235, 97, 80, 204 });
        }
    }
}

void GameRenderer::draw_marks_arrows(const GameModel& model) {
    const WindowState& ws = model.ws;
    const Vec2f half_cell = { ws.cell_size / 2, ws.cell_size / 2 };

    for (Move mark : model.mk.mark_buffer) {
        Vec2f from = cell_world_pos(mark.from, ws) + half_cell;
        Vec2f to = cell_world_pos(mark.to, ws) + half_cell;

        if (from != to) {
            int dx = std::abs(mark.to % 8 - mark.from % 8);
            int dy = std::abs(mark.to / 8 - mark.from / 8);
            bool is_knight_move = (dx == 2 && dy == 1) || (dx == 1 && dy == 2);

            draw_arrow(ws, from, to, is_knight_move);
        }
    }
}


void GameRenderer::draw_pieces(const GameModel& model) {
    RectShape cell({ model.ws.cell_size, model.ws.cell_size });
    cell.set_texture(&pieces);

    Move castle_rook = model.mm.is_castle ? model.mm.move.get_castle_rook_move() : null_move;
    has_top = false;

    for (int i = 0; i < 64; i++) {
        char p = model.board[i];
        if (p == '.') continue;

        int col = (tolower(p) == 'k') ? 0 :
                  (tolower(p) == 'q') ? 1 :
                  (tolower(p) == 'b') ? 2 :
                  (tolower(p) == 'n') ? 3 :
                  (tolower(p) == 'r') ? 4 : 5;
        int row = isupper(p) ? 0 : 1;

        Vec2f pos = cell_world_pos(i, model.ws);
        bool moving = (i == model.mm.move.from);
        bool rook_move = (i == castle_rook.from);

        if (moving && model.mm.mode == Mode::Dragging) {
            auto mp = Vec2f(Cursor::get_position());
            pos = mp - Vec2f{ model.ws.cell_size / 2, model.ws.cell_size / 2 };
        } 
        else if (rook_move || (moving && model.mm.mode == Mode::Sliding)) {
            Move mv = rook_move ? castle_rook : model.mm.move;
            auto start = cell_world_pos(mv.from, model.ws);
            auto end = cell_world_pos(mv.to, model.ws);
            pos = start + (end - start) * model.mm.slide_pct;
        }

        cell.set_position(pos);
        cell.set_texture_rect({ col * 133, row * 133, 133, 133 });

        if (moving) {
            top = cell;
            has_top = true;
        } 
        else {
            Draw::draw(cell);
        }
    }
}

void GameRenderer::draw_promotion_menu(const GameModel& model) {
    const char* options = model.board.is_white_to_move() ? "QRBN" : "qrbn";
    Vec2f pos = cell_world_pos(model.mm.move.to, model.ws);
    int dir = model.board.is_white_to_move() ? 1 : -1;

    for (int i = 0; i < 4; i++) {
        RectShape rect({ model.ws.cell_size, model.ws.cell_size });
        Vec2f curr_pos = { pos.x, pos.y + i * model.ws.cell_size * dir };
        rect.set_position(curr_pos);
        rect.set_fill_color({ 100, 100, 100, 200 });
        Draw::draw(rect);

        RectShape piece_rect({ model.ws.cell_size, model.ws.cell_size });
        piece_rect.set_texture(&pieces);
        int col = (std::tolower(options[i]) == 'q') ? 1 :
                  (std::tolower(options[i]) == 'r') ? 4 :
                  (std::tolower(options[i]) == 'b') ? 2 : 3;
        int row = std::isupper(options[i]) ? 0 : 1;
        piece_rect.set_texture_rect({ col * 133, row * 133, 133, 133 });
        piece_rect.set_position(curr_pos);
        Draw::draw(piece_rect);
    }
}

void GameRenderer::highlight(const WindowState& ws, int idx, Color color, bool fill) {
    if (idx < 0) return;
    RectShape r({ ws.cell_size, ws.cell_size });
    auto pos = cell_world_pos(idx, ws);
    r.set_position(pos);
    if (fill) {
        r.set_fill_color(color);
    }
    else {
        r.set_fill_color(Color::Transparent);
        r.set_outline_thickness(-5);
        r.set_outline_color(color);
    }
    Draw::draw(r);
}

void GameRenderer::draw_arrow(const WindowState& ws, Vec2f from, Vec2f to, bool lshape) {
    const float center_radius = 0.35f * ws.cell_size;
    const float arrow_thickness = 0.25f * ws.cell_size;
    const Color arrow_color = { 255, 170, 0, 204 };

    constexpr float pi = 3.1415926f;

    if (lshape) {

        bool is_horizontal = std::abs(to.x - from.x) >= std::abs(to.y - from.y);

        ConvShape head;
        head.set_fill_color(arrow_color);
        head.set_point_count(3);
        head.set_point(0, to);
        if (is_horizontal) {
            float y = to.y + (to.y >= from.y ? -1.0f : 1.0f) * center_radius;
            head.set_point(1, { to.x - arrow_thickness, y });
            head.set_point(2, { to.x + arrow_thickness, y });
        }
        else {
            float x = to.x + (to.x >= from.x ? -1.0f : 1.0f) * center_radius;
            head.set_point(1, { x, to.y - arrow_thickness });
            head.set_point(2, { x, to.y + arrow_thickness });
        }

        if (!is_horizontal) std::swap(from, to);
        float half_thickness = arrow_thickness / 2;
        Vec2f dir = { to.x >= from.x ? 1.0f : -1.0f, 
                      to.y >= from.y ? 1.0f : -1.0f };

        ConvShape horz;
        horz.set_point_count(4);
        horz.set_point(0, { from.x + dir.x * center_radius,  from.y - half_thickness });
        horz.set_point(1, { from.x + dir.x * center_radius,  from.y + half_thickness });
        horz.set_point(2, { to.x + dir.x * half_thickness, from.y + half_thickness });
        horz.set_point(3, { to.x + dir.x * half_thickness, from.y - half_thickness });
        horz.set_fill_color(arrow_color);

        ConvShape vert;
        vert.set_point_count(4);
        vert.set_point(0, { to.x - half_thickness, to.y - dir.y * center_radius });
        vert.set_point(1, { to.x + half_thickness, to.y - dir.y * center_radius });
        vert.set_point(2, { to.x + half_thickness, from.y + dir.y * half_thickness });
        vert.set_point(3, { to.x - half_thickness, from.y + dir.y * half_thickness });
        vert.set_fill_color(arrow_color);

        Draw::draw(horz);
        Draw::draw(vert);
        Draw::draw(head);

    }
    else {

        Vec2f dir = to - from;

        float angle_rad = std::atan2(dir.y, dir.x);
        float perp_rad = std::atan2(-dir.x, dir.y);
        float angle = angle_rad * 180.0f / pi;

        Vec2f center_shift = Vec2f{ std::cos(angle_rad), std::sin(angle_rad) } * center_radius;
        Vec2f thickness_shift = Vec2f{ std::cos(perp_rad), std::sin(perp_rad) } * arrow_thickness;
        Vec2f half_thickness_shift = thickness_shift / 2;

        Vec2f to_center = to;
        from += center_shift;
        to -= center_shift;

        ConvShape shaft;
        shaft.set_point_count(4);
        shaft.set_point(0, from - half_thickness_shift);
        shaft.set_point(1, from + half_thickness_shift);
        shaft.set_point(2, to + half_thickness_shift);
        shaft.set_point(3, to - half_thickness_shift);
        shaft.set_fill_color(arrow_color);

        ConvShape head;
        head.set_point_count(3);
        head.set_point(0, to - thickness_shift);
        head.set_point(1, to + thickness_shift);
        head.set_point(2, to_center);
        head.set_fill_color(arrow_color);

        Draw::draw(shaft);
        Draw::draw(head);

    }

}
