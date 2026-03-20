#pragma once
#include <stdint.h>

enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum { WHITE, BLACK, BOTH };

typedef struct
{
    uint64_t pieces[8];

    uint64_t colors[3];

    uint8_t castling_rights;
    int whose_turn;
    
} Position;

void init_starting_position(Position *position);

void print_position(Position *position);

uint64_t get_pawns_moves(Position *position, int color);