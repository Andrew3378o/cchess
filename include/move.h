#pragma once
#include <stdint.h>
#include "position.h"
#include "magic.h"

extern bitboard knight_moves[64];

bitboard get_pawns_moves(Position *position, int color);
bitboard get_pawns_attacks(Position *position, int color);
bitboard get_knights_moves(int sq, Position *position, int color);
bitboard get_bishop_moves(int sq, Position *position, int color);
bitboard get_rook_moves(int sq, Position *position, int color);
bitboard get_queen_moves(int sq, Position *position, int color);
bitboard get_kings_moves(Position *position, int color);
void init_knights_moves();
