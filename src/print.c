#include <stdio.h>
#include "print.h"
#include "constants.h"
#include "magic.h"

void print_position(Position *position) {
    printf("\n");

    for (int rank = 7; rank >= 0; rank--) {
        printf("%d ", rank + 1);

        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            bitboard bit = 1ULL << index;
            
            if ((rank + file) % 2 == 0) {
                printf("\033[100m");
            } else {
                printf("\033[47m");  
            }
            
            printf("\033[30m"); 

            char *piece_name = "   ";

            if (position->colors[WHITE] & bit) {
                if (position->pieces[PAWN]   & bit) piece_name = " ♙ ";
                else if (position->pieces[KNIGHT] & bit) piece_name = " ♘ ";
                else if (position->pieces[BISHOP] & bit) piece_name = " ♗ ";
                else if (position->pieces[ROOK]   & bit) piece_name = " ♖ ";
                else if (position->pieces[QUEEN]  & bit) piece_name = " ♕ ";
                else if (position->pieces[KING]   & bit) piece_name = " ♔ ";
            }
            else if (position->colors[BLACK] & bit) {
                if (position->pieces[PAWN]   & bit) piece_name = " ♟ ";
                else if (position->pieces[KNIGHT] & bit) piece_name = " ♞ ";
                else if (position->pieces[BISHOP] & bit) piece_name = " ♝ ";
                else if (position->pieces[ROOK]   & bit) piece_name = " ♜ ";
                else if (position->pieces[QUEEN]  & bit) piece_name = " ♛ ";
                else if (position->pieces[KING]   & bit) piece_name = " ♚ ";
            }

            printf("%s", piece_name);
            printf("\033[0m");
        }
        printf("\n");
    }
    printf("   a  b  c  d  e  f  g  h \n\n");
}

void print_moves(bitboard moves) {
    if (moves == 0) {
        printf("none\n");
        return;
    }

    while (moves) {
        
        int i = get_lsb_index(moves);
        int rank = (i / 8) + 1;      
        char file = 'a' + (i % 8);   
        
        printf("%c%d ", file, rank);
        
        moves &= (moves - 1);
    }
    printf("\n");
}