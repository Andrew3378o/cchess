#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "position.h"
#include "move.h"
#include "constants.h"
#include "print.h"
#include "zobrist.h"

bitboard game_history[1024];
int history_ply = 0;

int is_repetition(bitboard current_hash) {
    int count = 0;
    for (int i = 0; i < history_ply; i++) {
        if (game_history[i] == current_hash) {
            count++;
        }
    }
    return (count >= 2);
}

int main() {
    srand(12345);
    Position p;

    printf("Initializing starting position...\n\n");
    init_starting_position(&p);

    printf("Initializing knights moves...\n\n");
    init_knights_moves();

    printf("Initializing magic bitboards...\n\n");
    init_magic_bitboards();

    printf("Initializing zobrist keys...\n\n");
    init_zobrist();

    game_history[history_ply++] = generate_hash(&p);
    print_position(&p);

    while (1) {
        char input[10]; 

        printf("\nEnter move in LAN or 'q' to quit: ");
        
        if (scanf("%9s", input) != 1) {
            break;
        }

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            printf("Exiting the engine...\n");
            break;
        }

        Move m;
		int success = parse_move(input, &m, p.whose_turn);
        if ((m.from == m.to && m.castling == -1) || success == 1) {
            printf("Invalid move format! Please try again.\n");
            continue; 
        }

        success = is_legal(p, m);
        if(success == 1){
            printf("Illegal move! Please try again.\n");
            continue;
        }

        make_move(&p, m);
        print_position(&p);

        bitboard current_hash = generate_hash(&p);
        
        if (is_repetition(current_hash)) {
            printf("\nDRAW BY REPETITION!\n");
            break;
        }
        if (is_insufficient_material(&p)) {
            printf("\nDRAW BY INSUFFICIENT MATERIAL!\n");
            break;
        }
        
        game_history[history_ply++] = current_hash;

        Color current = p.whose_turn;
        Color enemy = (current == WHITE) ? BLACK : WHITE;

        bitboard king = p.pieces[KING] & p.colors[current];
        Square king_sq = (Square) get_lsb_index(king);

        int in_check = is_square_attacked(p, king_sq, enemy);
        int can_move = has_legal_moves(p);

        if(in_check && !can_move) {
            printf("MATE! GAME IS OVER. %s WON!\n", (current == WHITE) ? "BLACK" : "WHITE");
            break;
        }
        else if(!in_check && !can_move) {
            printf("STALEMATE! GAME IS OVER. DRAW!\n");
            break;
        }
        else if(in_check && can_move) {
            printf("CHECK!\n");
        }
    }

    return 0;
}