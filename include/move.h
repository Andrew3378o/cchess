#pragma once
#include <stdint.h>
#include "position.h"

uint64_t get_pawns_moves(Position *position, int color);
uint64_t get_pawns_attacks(Position *position, int color);

extern uint64_t knight_moves[64];
void init_knights_moves();

uint64_t get_kings_moves(Position *position, int color);