#include <stdio.h>
#include <string.h>
#include "position.h"
#include "move.h"
#include "constants.h"
#include "print.h"

int main() {
    Position p;

    init_starting_position(&p);

    printf("Initializing knights moves...\n\n");
    init_knights_moves();

    printf("Initializing magic bitboards...\n\n");
    init_magic_bitboards();

    print_position(&p);

    while (1) {
        char input[10]; 

        printf("\nEnter move in LAN (e.g., e2e4, 0-0, 0-0-0) or 'q' to quit: ");
        
        if (scanf("%9s", input) != 1) {
            break;
        }

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            printf("Exiting the engine...\n");
            break;
        }

        Move m;
		int success = parse_move(input, &m, p.whose_turn);
        if ((m.from == m.to && m.castling == -1) || success == -1) {
            printf("Invalid move format! Please try again.\n");
            continue; 
        }

        success = is_legal(p, m);
        if(success == -1){
            printf("Illegal move. Please try again.\n");
            continue;
        }

        make_move(&p, m);
        print_position(&p);
    }

    return 0;
}