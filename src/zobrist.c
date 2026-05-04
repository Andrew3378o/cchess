#include "zobrist.h"
#include "magic.h"

bitboard piece_keys[2][6][64];
bitboard castling_keys[16];
bitboard en_passant_keys[8];
bitboard side_key;

void init_zobrist(){
    for(int color = WHITE; color <= BLACK; color++){
        for(int piece = PAWN; piece <= KING; piece++){
            for(int sq = 0; sq < 64; sq++){
                piece_keys[color][piece][sq] = get_random_bitboard();
            }
        }
    }

    for(int i = 0; i < 16; i++){
        castling_keys[i] = get_random_bitboard();
    }

    for(int i = 0; i < 8; i++){
        en_passant_keys[i] = get_random_bitboard();
    }

    side_key = get_random_bitboard();
}

bitboard generate_hash(Position *p) {
    bitboard hash = 0ULL;

    for(int color = WHITE; color <= BLACK; color++){
        for(int piece = PAWN; piece <= KING; piece++){
            bitboard bitboard_copy = p->pieces[piece] & p->colors[color];
            
            while(bitboard_copy){
                int sq = get_lsb_index(bitboard_copy);
                hash ^= piece_keys[color][piece][sq]; 
                bitboard_copy &= bitboard_copy - 1;   
            }
        }
    }

    hash ^= castling_keys[p->castling_rights];

    if(p->en_passant != -1){
        int file = p->en_passant % 8;
        hash ^= en_passant_keys[file];
    }

    if(p->whose_turn == BLACK){
        hash ^= side_key;
    }

    return hash;
}