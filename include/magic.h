#pragma once
#include <stdlib.h>
#include "constants.h"

extern bitboard bishop_masks[64];
extern bitboard rook_masks[64];
extern int bishop_bits_numbers[64];
extern int rook_bits_numbers[64];
extern bitboard bishop_magic_numbers[64];
extern bitboard rook_magic_numbers[64];
extern bitboard bishop_attacks[64][512];
extern bitboard rook_attacks[64][4096];

bitboard get_rook_mask(int sq);
bitboard get_rook_on_the_fly(int sq, bitboard blocked);
bitboard get_bishop_mask(int sq);
bitboard get_bishop_on_the_fly(int sq, bitboard blocked);

bitboard set_occupancy(int index, int bits_number, bitboard attacks);
bitboard get_random_bitboard();
bitboard get_magic_candidate();
bitboard find_magic_number(int sq, int bits_number, int is_bishop);
void init_magic_bitboards();

static inline int get_lsb_index(bitboard b){
    if(b == 0) return -1;
    return __builtin_ctzll(b);
}

static inline int count_bits(bitboard b) {
    return __builtin_popcountll(b);
}