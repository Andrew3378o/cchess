#include <stdio.h>
#include "magic.h"

bitboard bishop_masks[64];
bitboard rook_masks[64];
int bishop_bits_numbers[64];
int rook_bits_numbers[64];
bitboard bishop_magic_numbers[64];
bitboard rook_magic_numbers[64];
bitboard bishop_attacks[64][512];
bitboard rook_attacks[64][4096];

bitboard get_rook_mask(int sq){
    bitboard mask = 0ULL;

    int r = sq / 8;
    int f = sq % 8;

    int i, j;

    for(i = r + 1; i <= 6; i++) mask |= (1ULL << (i * 8 + f));
    for(i = r - 1; i >= 1; i--) mask |= (1ULL << (i * 8 + f));
    for(j = f + 1; j <= 6; j++) mask |= (1ULL << (r * 8 + j));
    for(j = f - 1; j >= 1; j--) mask |= (1ULL << (r * 8 + j));

    return mask;
}

bitboard get_rook_on_the_fly(int sq, bitboard blocked){
    bitboard attacks = 0ULL;

    int r = sq / 8;
    int f = sq % 8;

    int i, j;
    bitboard bit;

    for(i = r + 1; i <= 7; i++){
        bit = (1ULL << (i * 8 + f));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(i = r - 1; i >= 0; i--){
        bit = (1ULL << (i * 8 + f));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(j = f + 1; j <= 7; j++){
        bit = (1ULL << (r * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(j = f - 1; j >= 0; j--){
        bit = (1ULL << (r * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    return attacks;
}

bitboard get_bishop_mask(int sq){
    bitboard mask = 0ULL;

    int r = sq / 8;
    int f = sq % 8;

    int i, j;

    for(i = r + 1, j = f + 1; i <= 6 && j <= 6; i++, j++) mask |= (1ULL << (i * 8 + j));
    for(i = r + 1, j = f - 1; i <= 6 && j >= 1; i++, j--) mask |= (1ULL << (i * 8 + j));
    for(i = r - 1, j = f + 1; i >= 1 && j <= 6; i--, j++) mask |= (1ULL << (i * 8 + j));
    for(i = r - 1, j = f - 1; i >= 1 && j >= 1; i--, j--) mask |= (1ULL << (i * 8 + j));

    return mask;
}

bitboard get_bishop_on_the_fly(int sq, bitboard blocked){
    bitboard attacks = 0ULL;

    int r = sq / 8;
    int f = sq % 8;

    int i, j;
    bitboard bit;

    for(i = r + 1, j = f + 1; i <= 7 && j <= 7; i++, j++){
        bit = (1ULL << (i * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(i = r + 1, j = f - 1; i <= 7 && j >= 0; i++, j--){
        bit = (1ULL << (i * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(i = r - 1, j = f + 1; i >= 0 && j <= 7; i--, j++){
        bit = (1ULL << (i * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    for(i = r - 1, j = f - 1; i >= 0 && j >= 0; i--, j--){
        bit = (1ULL << (i * 8 + j));
        attacks |= bit;
        if(blocked & bit) break;
    }

    return attacks;
}

bitboard set_occupancy(int index, int bits_number, bitboard attacks){
    bitboard occupancy = 0ULL;

    for(int i = 0; i < bits_number; i++){
        int sq = get_lsb_index(attacks);

        attacks &= attacks - 1;

        if(index & (1 << i)) occupancy |= (1ULL << sq);
    }

    return occupancy;
}

bitboard get_random_bitboard(){
    bitboard b1 = (bitboard)(rand()) & 0xFFFF;
    bitboard b2 = (bitboard)(rand()) & 0xFFFF;
    bitboard b3 = (bitboard)(rand()) & 0xFFFF;
    bitboard b4 = (bitboard)(rand()) & 0xFFFF;
    return b1 | (b2 << 16) | (b3 << 32) | (b4 << 48);
}

bitboard get_magic_candidate(){
    return get_random_bitboard() & get_random_bitboard() & get_random_bitboard();
}

bitboard find_magic_number(int sq, int bits_number, int is_bishop){
    bitboard occupancies[4096];
    bitboard attacks[4096];
    bitboard used_attacks[4096];

    bitboard mask = is_bishop ? get_bishop_mask(sq): get_rook_mask(sq);

    int indices = 1 << bits_number;

    for(int i = 0; i < indices; i++){
        occupancies[i] = set_occupancy(i, bits_number, mask);
        attacks[i] = is_bishop ? get_bishop_on_the_fly(sq, occupancies[i]): get_rook_on_the_fly(sq, occupancies[i]);
    }

    for(int i = 0; i < 100000000; i++){
        bitboard magic_number = get_magic_candidate();

        if(count_bits((mask * magic_number) & 0xFF00000000000000ULL) < 6) continue;

        for(int j = 0; j < 4096; j++) used_attacks[j] = 0ULL;

        int index, fail = 0;

        for(int j = 0; j < indices; j++){
            index = (int)((occupancies[j] * magic_number) >> (64 - bits_number));

            if(used_attacks[index] == 0ULL) used_attacks[index] = attacks[j];
            else if(used_attacks[index] != attacks[j]){
                fail = 1; 
                break;
            }
        }

        if(!fail) return magic_number;
    }

    printf("error: could not find magic number for square %d\n", sq);
    return 0ULL;
}

void init_magic_bitboards(){
    for(int sq = A1; sq <= H8; sq++){
        bishop_masks[sq] = get_bishop_mask(sq);
        bishop_bits_numbers[sq] = count_bits(bishop_masks[sq]);
        bishop_magic_numbers[sq] = find_magic_number(sq, bishop_bits_numbers[sq], 1);

        int bishop_occupancy_indices = 1 << bishop_bits_numbers[sq];

        for(int i = 0; i < bishop_occupancy_indices; i++){
            bitboard occupancy = set_occupancy(i, bishop_bits_numbers[sq], bishop_masks[sq]);
            int magic_index = (occupancy * bishop_magic_numbers[sq]) >> (64 - bishop_bits_numbers[sq]);
            bishop_attacks[sq][magic_index] = get_bishop_on_the_fly(sq, occupancy);
        }

        rook_masks[sq] = get_rook_mask(sq);
        rook_bits_numbers[sq] = count_bits(rook_masks[sq]);
        rook_magic_numbers[sq] = find_magic_number(sq, rook_bits_numbers[sq], 0);

        int rook_occupancy_indices = 1 << rook_bits_numbers[sq];

        for(int i = 0; i < rook_occupancy_indices; i++){
            bitboard occupancy = set_occupancy(i, rook_bits_numbers[sq], rook_masks[sq]);
            int magic_index = (occupancy * rook_magic_numbers[sq]) >> (64 - rook_bits_numbers[sq]);
            rook_attacks[sq][magic_index] = get_rook_on_the_fly(sq, occupancy);
        }
    }
}
