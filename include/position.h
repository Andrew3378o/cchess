#pragma once
#include <stdint.h>
#include "constants.h"

typedef struct
{
    bitboard pieces[8];

    bitboard colors[3];

    uint8_t castling_rights;

    Color whose_turn;

    int en_passant;
    
} Position;

void init_starting_position(Position *position);

