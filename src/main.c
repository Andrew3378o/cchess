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

	bitboard moves = get_pawns_moves(&p, WHITE);
	printf("possible pawns moves for white:\n");
	print_moves(moves);

	moves = get_knights_moves(B1, &p, WHITE) | get_knights_moves(G1, &p, WHITE);
	printf("possible knights moves for white:\n");
	print_moves(moves);
	return 0;
}