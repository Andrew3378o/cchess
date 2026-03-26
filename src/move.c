#include "move.h"
#include "constants.h"

bitboard get_pawns_moves(Position *position, int color) {
    bitboard pawns = position->pieces[PAWN] & position->colors[color];
    bitboard empty_squares = ~position->colors[BOTH];
    bitboard moves = 0ULL;

    if (color == WHITE) {
        bitboard single_push = (pawns << 8) & empty_squares;
        moves |= single_push;

        bitboard double_push = (single_push << 8) & empty_squares & RANK_4;
        moves |= double_push;
    } 
    else {
        bitboard single_push = (pawns >> 8) & empty_squares;
        moves |= single_push;

        bitboard double_push = (single_push >> 8) & empty_squares & RANK_5;
        moves |= double_push;
    }

    return moves;
}

bitboard get_pawns_attacks(Position *position, int color){
    bitboard pawns = position->pieces[PAWN] & position->colors[color];
    bitboard attacks = 0ULL;

    if(color == WHITE){
        attacks |= (pawns << 7) & ~FILE_H;
        attacks |= (pawns << 9) & ~FILE_A;
    }
    else{
        attacks |= (pawns >> 7) & ~FILE_A;
        attacks |= (pawns >> 9) & ~FILE_H;
    }
    return attacks;
}

bitboard knight_moves[64];
void init_knights_moves() {

    for(int i = A1; i <= H8; i++){
        bitboard bit = (1ULL << i);
        bitboard moves = 0;

        if(bit & ~FILE_A & ~FILE_B) moves |= (bit << 6);
        if(bit & ~FILE_H & ~FILE_G) moves |= (bit >> 6);
        if(bit & ~FILE_A & ~FILE_B) moves |= (bit >> 10);
        if(bit & ~FILE_H & ~FILE_G) moves |= (bit << 10);
        if(bit & ~FILE_A) moves |= (bit << 15);
        if(bit & ~FILE_H) moves |= (bit >> 15);
        if(bit & ~FILE_A) moves |= (bit >> 17);
        if(bit & ~FILE_H) moves |= (bit << 17);

        knight_moves[i] = moves;
    }
}

bitboard get_knights_moves(int sq, Position *position, int color){
    bitboard attacks = knight_moves[sq];

    bitboard moves = attacks & ~position->colors[color];
    
    return moves;
}

bitboard get_kings_moves(Position *position, int color){
    bitboard king = position->pieces[KING] & position->colors[color];
    bitboard moves = 0ULL;

    moves |= (king >> 8) | (king << 8);

    if(king & ~FILE_A){
        moves |= (king << 7) | (king >> 1) | (king >> 9);
    }

    if(king & ~FILE_H){
        moves |= (king << 9) | (king << 1) | (king >> 7);
    }

    return moves & ~position->colors[color];
}


bitboard get_bishop_moves(int sq, Position *position, int color){
    bitboard all_pieces = position->colors[BOTH];
    bitboard occupancy = all_pieces & bishop_masks[sq];
    
    int magic_index = (occupancy * bishop_magic_numbers[sq]) >> (64 - bishop_bits_numbers[sq]);
    bitboard raw = bishop_attacks[sq][magic_index];

    return raw & ~position->colors[color];
}

bitboard get_rook_moves(int sq, Position *position, int color){
    bitboard all_pieces = position->colors[BOTH];
    bitboard occupancy = all_pieces & rook_masks[sq];

    int magic_index = (occupancy * rook_magic_numbers[sq]) >> (64 - rook_bits_numbers[sq]);
    bitboard raw = rook_attacks[sq][magic_index];

    return raw & ~position->colors[color];
}

bitboard get_queen_moves(int sq, Position *position, int color){
    return get_bishop_moves(sq, position, color) | get_rook_moves(sq, position, color);
}