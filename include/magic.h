#pragma once
#include <stdlib.h>
#include "constants.h"

bitboard get_bishop_mask(Square sq);
bitboard get_bishop_on_the_fly(Square sq, bitboard blocked);
bitboard *get_bishop_attacks(Square sq);
int get_bishop_magic_number(Square sq);
int get_bishop_bits_number(Square sq);

bitboard get_rook_mask(Square sq);
bitboard get_rook_on_the_fly(Square sq, bitboard blocked);
bitboard *get_rook_attacks(Square sq);
int get_rook_magic_number(Square sq);
int get_rook_bits_number(Square sq);

bitboard set_occupancy(int index, int bits_number, bitboard attacks);
bitboard get_random_bitboard();
bitboard get_magic_candidate();
bitboard find_magic_number(Square sq, int bits_number, int is_bishop);
void init_magic_bitboards();

#if defined(_MSC_VER)
    #include <intrin.h>

    static inline int count_bits(bitboard b) {
        return (int)__popcnt64(b);
    }

    static inline int get_lsb_index(bitboard b) {
        unsigned long idx;
        if (_BitScanForward64(&idx, b)) {
            return (int)idx;
        }
        return -1;
    }

#elif defined(__GNUC__) || defined(__clang__)

    static inline int count_bits(bitboard b) {
        return __builtin_popcountll(b);
    }

    static inline int get_lsb_index(bitboard b) {
        if (b == 0) return -1;
        return __builtin_ctzll(b);
    }

#else
    #error "Unsupported compiler"
#endif