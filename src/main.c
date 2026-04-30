#include <stdio.h>
#include <string.h>
#include "position.h"
#include "move.h"
#include "constants.h"
#include "print.h"

int main() {
    Position p;

    init_starting_position(&p);
    init_knights_moves();

    print_position(&p);

    while (1) {
        char input[10]; 

        printf("\nEnter move (e.g., e2e4) or 'q' to quit: ");
        
        if (scanf("%9s", input) != 1) {
            break;
        }

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            printf("Exiting the engine...\n");
            break;
        }

        Move m;
		int success = parse_move(input, &m);
        if ((m.from == m.to && m.castling == -1) || success == -1) {
            printf("Invalid move format! Please try again.\n");
            continue; 
        }

        success = make_move(&p, m);
        if (success) {
            print_position(&p);
        } else {
            printf("Illegal move! Try again.\n");
        }
    }

    return 0;
}