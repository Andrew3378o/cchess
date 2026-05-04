#pragma once
#include "constants.h"
#include "position.h"

extern bitboard piece_keys[2][6][64];
extern bitboard castling_keys[16];
extern bitboard en_passant_keys[8];
extern bitboard side_key;

void init_zobrist();
bitboard generate_hash(Position *p);
