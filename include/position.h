#pragma once
#include <stdint.h>
#include "constants.h"

typedef struct
{
    bitboard pieces[8];

    bitboard colors[3];

    uint8_t castling_rights;
    int whose_turn;
    
} Position;

void init_starting_position(Position *position);

