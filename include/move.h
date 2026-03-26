#pragma once
#include <stdint.h>
#include "position.h"

bitboard get_pawns_moves(Position *position, int color);
bitboard get_pawns_attacks(Position *position, int color);

extern bitboard knight_moves[64];
void init_knights_moves();
bitboard get_knights_moves(int sq, Position *position, int color);

bitboard get_kings_moves(Position *position, int color);