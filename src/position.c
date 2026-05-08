#include <string.h>
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

    position->en_passant = -1;
}

int is_insufficient_material(Position *p) {
    if (p->pieces[PAWN] || p->pieces[ROOK] || p->pieces[QUEEN]) {
        return 0;
    }

    int w_knights = count_bits(p->pieces[KNIGHT] & p->colors[WHITE]);
    int b_knights = count_bits(p->pieces[KNIGHT] & p->colors[BLACK]);
    int w_bishops = count_bits(p->pieces[BISHOP] & p->colors[WHITE]);
    int b_bishops = count_bits(p->pieces[BISHOP] & p->colors[BLACK]);

    int total_minors = w_knights + b_knights + w_bishops + b_bishops;

    if (total_minors == 0) return 1;

    if (total_minors == 1) return 1;

    if (w_knights == 0 && b_knights == 0 && w_bishops == 1 && b_bishops == 1) {
        bitboard w_bishop = p->pieces[BISHOP] & p->colors[WHITE];
        bitboard b_bishop = p->pieces[BISHOP] & p->colors[BLACK];
        
        bitboard light_squares = 0x55AA55AA55AA55AAULL;
        
        int wb_is_light = (w_bishop & light_squares) != 0;
        int bb_is_light = (b_bishop & light_squares) != 0;

        if (wb_is_light == bb_is_light) return 1;
    }

    return 0;
}
