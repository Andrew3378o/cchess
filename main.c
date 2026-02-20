#include <stdio.h>
#include "position.h"

int main(){
    Position p;
    init_starting_position(&p);
    print_position(&p);
    return 0;
}