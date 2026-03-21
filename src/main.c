#include <stdio.h>
#include "position.h"
#include "move.h"
#include "constants.h"

int main(){
	Position p;
	init_starting_position(&p);
	init_knights_moves();
	print_position(&p);
	return 0;
}