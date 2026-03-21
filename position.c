#include <stdio.h>
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
}

void print_position(Position *position){
    printf("\n");

    for(int rank = 7; rank >=0; rank--){
        printf("%d ", rank + 1);

        for(int file = 0; file < 8; file++){
            int index = rank * 8 + file;

            uint64_t bit = 1ULL << index;
            char *piece_name = "[  ]";

            if (position->colors[WHITE] & bit) {
                if (position->pieces[PAWN]   & bit) piece_name = "[+P]";
                else if (position->pieces[KNIGHT] & bit) piece_name = "[+N]";
                else if (position->pieces[BISHOP] & bit) piece_name = "[+B]";
                else if (position->pieces[ROOK]   & bit) piece_name = "[+R]";
                else if (position->pieces[QUEEN]  & bit) piece_name = "[+Q]";
                else if (position->pieces[KING]   & bit) piece_name = "[+K]";
            }
            else if (position->colors[BLACK] & bit){
                if (position->pieces[PAWN]   & bit) piece_name = "[-P]";
                else if (position->pieces[KNIGHT] & bit) piece_name = "[-N]";
                else if (position->pieces[BISHOP] & bit) piece_name = "[-B]";
                else if (position->pieces[ROOK]   & bit) piece_name = "[-R]";
                else if (position->pieces[QUEEN]  & bit) piece_name = "[-Q]";
                else if (position->pieces[KING]   & bit) piece_name = "[-K]";
            }

            printf(piece_name);
        }
        printf("\n");
    }
    printf("  [ a][ b][ c][ d][ e][ f][ g][ h]\n\n");
}
