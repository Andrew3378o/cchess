#include <stdio.h>
#include <intrin.h>
#include "print.h"
#include "constants.h"

void print_position(Position *position){
    printf("\n");

    for(int rank = 7; rank >=0; rank--){
        printf("%d ", rank + 1);

        for(int file = 0; file < 8; file++){
            int index = rank * 8 + file;

            bitboard bit = 1ULL << index;
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

void print_moves(bitboard moves) {
    unsigned long i;
    
    if (moves == 0) {
        printf("none\n");
        return;
    }

    while (moves) {
        _BitScanForward64(&i, moves);
        
        int rank = (i / 8) + 1;      
        char file = 'a' + (i % 8);   
        
        printf("%c%d ", file, rank);
        
        moves &= (moves - 1);
    }
    printf("\n");
}