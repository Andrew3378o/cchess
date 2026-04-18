#pragma once
#include <stdint.h>
#include "position.h"
#include "magic.h"

extern bitboard knight_moves[64];

bitboard get_pawns_moves(Position *position, Color color);
bitboard get_pawns_attacks(Position *position, Color color);
bitboard get_knights_moves(Square sq, Position *position, Color color);
bitboard get_bishop_moves(Square sq, Position *position, Color color);
bitboard get_rook_moves(Square sq, Position *position, Color color);
bitboard get_queen_moves(Square sq, Position *position, Color color);
bitboard get_kings_moves(Position *position, Color color);
void init_knights_moves();
