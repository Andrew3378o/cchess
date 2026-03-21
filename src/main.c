#include <stdio.h>
#include "position.h"
#include "move.h"
#include "constants.h"
#include "print.h"

int main(){
	Position p;

	init_starting_position(&p);
	init_knights_moves();

	print_position(&p);

	uint64_t moves = get_pawns_moves(&p, WHITE) | get_pawns_moves(&p, BLACK);
	printf("possible pawns moves for both colors:\n");
	print_moves(moves);

	moves = get_knights_moves(B1, &p, WHITE) | get_knights_moves(G1, &p, WHITE) | get_knights_moves(B8, &p, BLACK) | get_knights_moves(G8, &p, BLACK);
	printf("possible knights moves for both colors:\n");
	print_moves(moves);
	return 0;
}