#pragma once
#include "board.hpp"

bool is_piece_white(char piece) {
    return (bool) std::isupper(piece);
}

Move::Move(int _from, int _to, char _promotion) 
    : from(_from), to(_to), promotion(_promotion) {}

Move Move::get_castle_rook_move() const {
    int row = from / 8;
    // King went two squares right → kingside castle
    if (to - from == 2) {
        int rook_from = row * 8 + 7;
        int rook_to = row * 8 + 5;
        return Move{ rook_from, rook_to };
    }
    // King went two squares left → queenside castle
    else { // to - from == -2
        int rook_from = row * 8 + 0;
        int rook_to = row * 8 + 3;
        return Move{ rook_from, rook_to };
    }
}

bool Move::operator<(const Move& other) const {
    return from < other.from || (from == other.from && to < other.to);
}

Board::Board() {
    parse_fen(start_fen);
}

Board::Board(std::string_view fen) {
    parse_fen(fen);
}

void Board::make_move(Move m) {
    char piece = board[m.from];
    char lp = std::tolower(piece);
    char target = board[m.to];

    // Halfmove clock: reset on capture or pawn move
    if (lp == 'p' || target != '.') {
        halfmove = 0;
    }
    else {
        halfmove++;
    }

    // Move piece
    board[m.to] = (m.promotion && (lp == 'p') &&
        (m.to / 8 == 0 || m.to / 8 == 7)) ?
        (white_to_move ? std::toupper(m.promotion) : std::tolower(m.promotion)) :
        piece;
    board[m.from] = '.';

    // Castling: move rook
    king_passant = -1;
    if (lp == 'k') {
        Move rook_move = std::abs(m.to - m.from) == 2 ? 
                        m.get_castle_rook_move() : null_move;
        if (!rook_move.is_null()) {
            board[rook_move.to] = board[rook_move.from];
            board[rook_move.from] = '.';
            // update king passant square
            king_passant = (m.from + m.to) / 2;
        }
        // remove castling rights for this side
        castling[white_to_move ? 0 : 2] = false;
        castling[white_to_move ? 1 : 3] = false;
    }

    // Remove castling rights if rook moved or was captured
    if (lp == 'r') {
        if (m.from == 56) castling[1] = false;
        if (m.from == 63) castling[0] = false;
        if (m.from == 0) castling[3] = false;
        if (m.from == 7) castling[2] = false;
    }
    if (std::tolower(target) == 'r') {
        if (m.to == 56) castling[1] = false;
        if (m.to == 63) castling[0] = false;
        if (m.to == 0) castling[3] = false;
        if (m.to == 7) castling[2] = false;
    }

    // En passant capture
    if (lp == 'p' && m.to == en_passant) {
        int cap_sq = white_to_move ? m.to + 8 : m.to - 8;
        board[cap_sq] = '.';
    }

    // Set new en passant square
    if (lp == 'p' && std::abs(m.to - m.from) == 16) {
        en_passant = white_to_move ? m.from - 8 : m.from + 8;
    }
    else {
        en_passant = -1;
    }

    // Fullmove increment after Black's move
    if (!white_to_move) fullmove++;

    // Toggle side
    white_to_move = !white_to_move;
}

std::vector<Move> Board::gen_moves(bool white) {
    std::vector<Move> moves;
    moves.reserve(128);
    static const int knight_dirs[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
    static const int bishop_dirs[4] = { -9, -7, 7, 9 };
    static const int rook_dirs[4] = { -8, -1, 1, 8 };
    static const int queen_dirs[8] = { -9, -8, -7, -1, 1, 7, 8, 9 };

    for (int i = 0; i < 64; i++) {
        char p = board[i];
        if (p == '.' || is_piece_white(p) != white) continue;
        bool is_white = is_piece_white(p);
        char piece = std::tolower(p);
        int forward = is_white ? -8 : 8;

        switch (piece) {
        case 'p': {
            int one = i + forward;
            if (on_board(one) && board[one] == '.') {
                add_pawn_move(i, one, moves, is_white);
                int start_rank = is_white ? 6 : 1;
                int two = one + forward;
                if (rank(i) == start_rank && board[two] == '.') {
                    moves.push_back({ i, two, 0 });
                }
            }
            for (int dx : {-1, 1}) {
                int cap = i + forward + dx;
                if (on_board(cap) && file_diff(i, cap) == 1) {
                    if (board[cap] != '.' && is_piece_white(board[cap]) != is_white) {
                        add_pawn_move(i, cap, moves, is_white);
                    }
                    else if (cap == en_passant) {
                        moves.push_back({ i, cap, 0 });
                    }
                }
            }
            break;
        }
        case 'n': {
            for (int d : knight_dirs) {
                int to = i + d;
                if (!on_board(to) || file_diff(i, to) > 2) continue;
                if (board[to] == '.' || is_piece_white(board[to]) != is_white)
                    moves.push_back({ i, to, 0 });
            }
            break;
        }
        case 'b': case 'r': case 'q': {
            const int* dirs;
            int dir_count;
            if (piece == 'b') { dirs = bishop_dirs; dir_count = 4; }
            else if (piece == 'r') { dirs = rook_dirs; dir_count = 4; }
            else { dirs = queen_dirs; dir_count = 8; }
            for (int di = 0; di < dir_count; ++di) {
                int d = dirs[di];
                int to = i;
                while (true) {
                    int prev = to;
                    to += d;
                    if (!on_board(to) || file_diff(prev, to) > 1) break;
                    if (board[to] == '.') {
                        moves.push_back({ i, to, 0 });
                    }
                    else {
                        if (is_piece_white(board[to]) != is_white)
                            moves.push_back({ i, to, 0 });
                        break;
                    }
                }
            }
            break;
        }
        case 'k': {
            for (int d : queen_dirs) {
                int to = i + d;
                if (!on_board(to) || file_diff(i, to) > 1) continue;
                if (board[to] == '.' || is_piece_white(board[to]) != is_white)
                    moves.push_back({ i, to, 0 });
            }
            add_castles(i, is_white, moves);
            break;
        }
        }
    }

    return moves;
}

std::vector<Move> Board::gen_legal_moves(bool white) {
    std::vector<Move> legal;
    legal.reserve(128);
    for (auto m : gen_moves(white)) {
        Board copy = *this;
        copy.make_move(m);
        if (!copy.is_winning(!white))
            legal.push_back(m);
    }
    return legal;
}

bool Board::is_winning(bool white) {
    for (Move move : gen_moves(white)) {
        // Can capture the king / King passant
        if (std::tolower(board[move.to]) == 'k' || (white == white_to_move &&
            king_passant >= 0 && std::abs(move.to - king_passant) < 2)) {
            return true;
        }
    }
    return false;
}

bool Board::is_promotion_like(const Move& move) const {
    char piece = board[move.from];
    int row = move.to / 8;
    return (std::tolower(piece) == 'p') && (row == 0 || row == 7);
}


void Board::add_pawn_move(int from, int to, std::vector<Move>& mvs, bool white, char capture) const {
    int r = to / 8;
    if ((white && r == 0) || (!white && r == 7)) {
        for (char prom : {'q', 'r', 'b', 'n'}) {
            mvs.emplace_back(from, to, white ? std::toupper(prom) : prom);
        }
    }
    else {
        mvs.emplace_back(from, to, 0);
    }
}

void Board::add_castles(int king_sq, bool white, std::vector<Move>& mvs) const {
    if (white && can_castle_kingside(true) && board[61] == '.' && board[62] == '.') 
        mvs.emplace_back(king_sq, 62, 0);
    if (white && can_castle_queenside(true) && board[57] == '.' && board[58] == '.' && board[59] == '.')
        mvs.emplace_back(king_sq, 58, 0);
    if (!white && can_castle_kingside(false) && board[5] == '.' && board[6] == '.') 
        mvs.emplace_back(king_sq, 6, 0);
    if (!white && can_castle_queenside(false) && board[1] == '.' && board[2] == '.' && board[3] == '.') 
        mvs.emplace_back(king_sq, 2, 0);
}

void Board::parse_fen(std::string_view fen) {
    // Parse FEN string
    std::istringstream iss;
    iss.str(std::string(fen));
    std::string board_part, side, castle, ep;
    iss >> board_part >> side >> castle >> ep >> halfmove >> fullmove;

    // Parse board layout
    int idx = 0;
    for (char c : board_part) {
        if (c == '/') continue;
        if (std::isdigit(c)) {
            int empties = c - '0';
            for (int i = 0; i < empties; i++) board[idx++] = '.';
        }
        else {
            board[idx++] = c;
        }
    }

    // Side to move
    white_to_move = (side == "w");

    // Castling rights
    castling = { false, false, false, false };
    if (castle.find('K') != std::string::npos) castling[0] = true;
    if (castle.find('Q') != std::string::npos) castling[1] = true;
    if (castle.find('k') != std::string::npos) castling[2] = true;
    if (castle.find('q') != std::string::npos) castling[3] = true;

    // En passant
    if (ep == "-") {
        en_passant = -1;
    }
    else {
        int file = ep[0] - 'a';
        int rank = ep[1] - '1';
        en_passant = rank * 8 + file;
    }
}
