#include "move.h"
#include "constants.h"

uint64_t get_pawns_moves(Position *position, int color) {
    uint64_t pawns = position->pieces[PAWN] & position->colors[color];
    uint64_t empty_squares = ~position->colors[BOTH];
    uint64_t moves = 0ULL;

    if (color == WHITE) {
        uint64_t single_push = (pawns << 8) & empty_squares;
        moves |= single_push;

        uint64_t double_push = (single_push << 8) & empty_squares & RANK_4;
        moves |= double_push;
    } 
    else {
        uint64_t single_push = (pawns >> 8) & empty_squares;
        moves |= single_push;

        uint64_t double_push = (single_push >> 8) & empty_squares & RANK_5;
        moves |= double_push;
    }

    return moves;
}

uint64_t knight_moves[64];
void init_knights_moves() {

    for(int i = A1; i <= H8; i++){
        uint64_t bit = (1ULL << i);
        uint64_t moves = 0;

        if(bit & ~FILE_A & ~FILE_B) moves |= (bit << 6);
        if(bit & ~FILE_H & ~FILE_G) moves |= (bit >> 6);
        if(bit & ~FILE_A & ~FILE_B) moves |= (bit >> 10);
        if(bit & ~FILE_H & ~FILE_G) moves |= (bit << 10);
        if(bit & ~FILE_A) moves |= (bit << 15);
        if(bit & ~FILE_H) moves |= (bit >> 15);
        if(bit & ~FILE_A) moves |= (bit >> 17);
        if(bit & ~FILE_H) moves |= (bit << 17);

        knight_moves[i] = moves;
    }
}
