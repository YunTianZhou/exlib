#pragma once
#include <string>
#include <string_view>
#include <sstream>
#include <cctype>
#include <array>
#include <vector>
#include <unordered_map>

const int N = -8, E = 1, S = 10, W = -1;

const std::string_view start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

bool is_piece_white(char piece);

struct Move {
    int from = -1;
    int to = -1;
    char promotion = 0;  // e.g., 'q', 'r', 'b', 'n'

    Move(int _from, int _to, char _promotion = 0);

    bool is_null() const { return from == -1 || to == -1; }

    Move get_castle_rook_move() const;

    bool operator<(const Move& other) const;
};

inline const Move null_move = { -1, -1, 0 };

struct Board {
    Board();
    Board(std::string_view fen);

    void make_move(Move m);
    std::vector<Move> gen_moves(bool white);
    std::vector<Move> gen_legal_moves(bool white);
    bool is_winning(bool white);
    bool is_promotion_like(const Move& move) const;

    char& operator[](int pos) { return board[pos]; }
    const char& operator[](int pos) const { return board[pos]; }

    bool is_white_to_move() const { return white_to_move; }
    bool can_castle_kingside(bool white) const { return white ? castling[0] : castling[2]; }
    bool can_castle_queenside(bool white) const { return white ? castling[1] : castling[3]; }
    int en_passant_square() const { return en_passant; }
    int king_passant_square() const { return king_passant; }
    int halfmove_clock() const { return halfmove; }
    int fullmove_number() const { return fullmove; }

private:
    std::array<char, 64> board{};
    bool white_to_move = true;
    // Castling rights: [white K, white Q, black K, black Q]
    std::array<bool, 4> castling{};
    // En passant target square (0-63), or -1 if none
    int en_passant = -1;
    // King passant square (prev castling square), or -1 if none
    int king_passant = -1;
    int halfmove = 0;
    int fullmove = 1;

    // Helpers
    static bool on_board(int sq) { return sq >= 0 && sq < 64; }
    static int file(int sq) { return sq & 7; }
    static int rank(int sq) { return sq >> 3; }
    static int file_diff(int a, int b) { return std::abs(file(a) - file(b)); }

    void add_pawn_move(int from, int to, std::vector<Move>& mvs, bool white, char capture = 0) const;
    void add_castles(int king_sq, bool white, std::vector<Move>& mvs) const;
    void parse_fen(std::string_view fen);
};
