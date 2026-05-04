#pragma once
#include <stdint.h>
#include "position.h"
#include "magic.h"

typedef struct {
    Square from;
    Square to;
    Piece piece;
    Color color;
    int castling;
} Move;

bitboard get_pawns_moves(Position position, Color color);
bitboard get_pawns_attacks(Position position, Color color);
bitboard get_knights_moves(Square sq, Position position, Color color);
bitboard get_bishop_moves(Square sq, Position position, Color color);
bitboard get_rook_moves(Square sq, Position position, Color color);
bitboard get_queen_moves(Square sq, Position position, Color color);
bitboard get_kings_moves(Position position, Color color);

int parse_move(const char *input, Move *move, Color side);
int make_move(Position *position, Move move);
int is_legal(Position position, Move move);
int is_square_attacked(Position position, Square sq, Color attacker);
int has_legal_moves(Position position);
void init_knights_moves();
