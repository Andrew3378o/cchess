#pragma once
#include <stdint.h>
#include "constants.h"

typedef struct
{
    bitboard pieces[8];
    bitboard colors[3];
    uint8_t castling_rights;
    Color whose_turn;
    Square en_passant;
} Position;

void init_starting_position(Position *position);
int is_insufficient_material(Position *p);

