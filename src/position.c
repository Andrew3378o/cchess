#include "position.h"
#include "constants.h"

void init_starting_position(Position *position){
    memset(position, 0, sizeof(Position));

    position->pieces[PAWN] = 0x00FF00000000FF00ULL;
    position->pieces[KNIGHT] = 0x4200000000000042ULL;
    position->pieces[BISHOP] = 0x2400000000000024ULL; 
    position->pieces[ROOK]   = 0x8100000000000081ULL; 
    position->pieces[QUEEN]  = 0x0800000000000008ULL; 
    position->pieces[KING]   = 0x1000000000000010ULL; 

    position->colors[WHITE]  = 0x000000000000FFFFULL; 
    position->colors[BLACK]  = 0xFFFF000000000000ULL;
    position->colors[BOTH] = position->colors[WHITE] | position->colors[BLACK];

    position->whose_turn = WHITE;
    position->castling_rights = 0b1111;
}
