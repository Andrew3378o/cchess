#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "move.h"
#include "constants.h"
#include "magic.h"

static bitboard knight_moves[64];
static const uint8_t castling_rights_update[64] = {
    13, 15, 15, 15, 12, 15, 15, 14, 
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 
     7, 15, 15, 15,  3, 15, 15, 11 
};

bitboard get_pawns_moves(Position position, Color color) {
    bitboard pawns = position.pieces[PAWN] & position.colors[color];
    bitboard empty_squares = ~position.colors[BOTH];
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

bitboard get_pawns_attacks(Position position, Color color){
    bitboard pawns = position.pieces[PAWN] & position.colors[color];
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

bitboard get_knights_moves(Square sq, Position position, Color color){
    bitboard attacks = knight_moves[sq];
    bitboard moves = attacks & ~position.colors[color];
    return moves;
}

bitboard get_kings_moves(Position position, Color color){
    bitboard king = position.pieces[KING] & position.colors[color];
    bitboard moves = 0ULL;

    moves |= (king >> 8) | (king << 8);

    if(king & ~FILE_A){
        moves |= (king << 7) | (king >> 1) | (king >> 9);
    }

    if(king & ~FILE_H){
        moves |= (king << 9) | (king << 1) | (king >> 7);
    }

    return moves & ~position.colors[color];
}

bitboard get_bishop_moves(Square sq, Position position, Color color){
    bitboard all_pieces = position.colors[BOTH];
    bitboard occupancy = all_pieces & get_bishop_mask(sq);
    
    int magic_index = (occupancy * get_bishop_magic_number(sq)) >> (64 - get_bishop_bits_number(sq));
    bitboard raw = get_bishop_attacks(sq)[magic_index];

    return raw & ~position.colors[color];
}

bitboard get_rook_moves(Square sq, Position position, Color color){
    bitboard all_pieces = position.colors[BOTH];
    bitboard occupancy = all_pieces & get_rook_mask(sq);

    int magic_index = (occupancy * get_rook_magic_number(sq)) >> (64 - get_rook_bits_number(sq));
    bitboard raw = get_rook_attacks(sq)[magic_index];

    return raw & ~position.colors[color];
}

bitboard get_queen_moves(Square sq, Position position, Color color){
    return get_bishop_moves(sq, position, color) | get_rook_moves(sq, position, color);
}

int parse_move(const char *input, Move *move, Color side){
    move->from = A1;
    move->to = A1;
    move->piece = NONE;
    move->castling = -1; 
    move->color = side;

    if(input == NULL){
        return 1;
    }
    else if(strcmp(input, "O-O") == 0){
        move->castling = 0; 
    }
    else if(strcmp(input, "O-O-O") == 0){
        move->castling = 1; 
    }
    else if(strlen(input) >= 4){
        int from_file = tolower(input[0]) - 'a';
        int from_rank = input[1] - '1';
        int to_file = tolower(input[2]) - 'a';
        int to_rank = input[3] - '1';

        if(from_file < 0 || from_file > 7 || from_rank < 0 || from_rank > 7 ||
           to_file < 0 || to_file > 7 || to_rank < 0 || to_rank > 7) return 1;

        move->from = (Square)(from_rank * 8 + from_file);
        move->to = (Square)(to_rank * 8 + to_file);
        move->piece = PAWN;

        if(strlen(input) >= 5){
            char promotion = tolower(input[4]);
            switch (promotion){
                case 'q': move->piece = QUEEN; break;
                case 'r': move->piece = ROOK; break;
                case 'b': move->piece = BISHOP; break;
                case 'n': move->piece = KNIGHT; break;
                default: break;
            }
        }
    }
    else{
        return 1;
    }

    return 0;
}

int make_move(Position *position, Move move) {
    Color side = position->whose_turn;
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    if (move.castling != -1) {
        if (side == WHITE) {
            if (move.castling == 0) {
                position->pieces[KING] &= ~(1ULL << E1);
                position->pieces[ROOK] &= ~(1ULL << H1);
                position->colors[WHITE] &= ~((1ULL << E1) | (1ULL << H1));
                
                position->pieces[KING] |= (1ULL << G1);
                position->pieces[ROOK] |= (1ULL << F1);
                position->colors[WHITE] |= ((1ULL << G1) | (1ULL << F1));
                
            } 
            else {
                position->pieces[KING] &= ~(1ULL << E1);
                position->pieces[ROOK] &= ~(1ULL << A1);
                position->colors[WHITE] &= ~((1ULL << E1) | (1ULL << A1));
                
                position->pieces[KING] |= (1ULL << C1);
                position->pieces[ROOK] |= (1ULL << D1);
                position->colors[WHITE] |= ((1ULL << C1) | (1ULL << D1));
            }
            position->castling_rights &= ~(WK | WQ); 
            
        } 
        else {
            if (move.castling == 0) { 
                position->pieces[KING] &= ~(1ULL << E8);
                position->pieces[ROOK] &= ~(1ULL << H8);
                position->colors[BLACK] &= ~((1ULL << E8) | (1ULL << H8));
                
                position->pieces[KING] |= (1ULL << G8);
                position->pieces[ROOK] |= (1ULL << F8);
                position->colors[BLACK] |= ((1ULL << G8) | (1ULL << F8));
            } 
            else { 
                position->pieces[KING] &= ~(1ULL << E8);
                position->pieces[ROOK] &= ~(1ULL << A8);
                position->colors[BLACK] &= ~((1ULL << E8) | (1ULL << A8));
                
                position->pieces[KING] |= (1ULL << C8);
                position->pieces[ROOK] |= (1ULL << D8);
                position->colors[BLACK] |= ((1ULL << C8) | (1ULL << D8));
            }
            position->castling_rights &= ~(BK | BQ);
        }

        position->colors[BOTH] = position->colors[WHITE] | position->colors[BLACK];
        position->whose_turn = enemy;
    
        return 0; 
    }

    bitboard from_bit = 1ULL << move.from;
    bitboard to_bit = 1ULL << move.to;

    Piece moving_piece = NONE;
    for(int p = PAWN; p <= KING; p++){
        if(position->pieces[p] & from_bit){
            moving_piece = p;
            break;
        }
    }

    position->pieces[moving_piece] &= ~from_bit;
    position->colors[side] &= ~from_bit;

    if (moving_piece == PAWN && move.to == position->en_passant){
        Square pawn_sq = (side == WHITE) ? (move.to - 8) : (move.to + 8);
        bitboard captured_bit = 1ULL << pawn_sq;

        position->pieces[PAWN] &= ~captured_bit;
        position->colors[enemy] &= ~ captured_bit;
    }

    if(position->colors[enemy] & to_bit){
        for(int p = PAWN; p <= KING; p++){
            if(position->pieces[p] & to_bit){
                position->pieces[p] &= ~to_bit;
                break;
            }
        }
        position->colors[enemy] &= ~to_bit;
    }

    if(moving_piece == PAWN && move.piece != PAWN && move.piece != NONE){
        position->pieces[move.piece] |= to_bit;
    }
    else{
        position->pieces[moving_piece] |= to_bit;
    }
    position->colors[side] |= to_bit;

    position->colors[BOTH] = position->colors[WHITE] | position->colors[BLACK];

    position->whose_turn = enemy;

    position->castling_rights &= castling_rights_update[move.from];
    position->castling_rights &= castling_rights_update[move.to];

    position->en_passant = -1;
    if(moving_piece == PAWN){
        int diff = (int)move.to - (int)move.from;
        if(diff == 16){
            position->en_passant = move.from + 8;
        }
        else if(diff == -16){
            position->en_passant = move.from - 8;
        }
    }

    return 0;
}

int is_legal(Position position, Move move) {
    Color side = position.whose_turn;
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    if (move.castling != -1) {
        Square king_sq = (side == WHITE) ? E1 : E8;
        if (is_square_attacked(position, king_sq, enemy)) {
            //printf("[DEBUG] castling illegal - king is currently in check.\n");
            return 1;
        }
        if (side == WHITE) {
            if (move.castling == 0) {
                //printf("[DEBUG] castling rights - %d\n", position.castling_rights);
                if (!(position.castling_rights & WK)) {
                    //printf("[DEBUG] missing white king-side castling rights (WK).\n");
                    return 1;
                }
                if (position.colors[BOTH] & ((1ULL << F1) | (1ULL << G1))) {
                    //printf("[DEBUG] castling path F1-G1 is blocked by pieces.\n");
                    return 1;
                }
                if (is_square_attacked(position, F1, enemy) || is_square_attacked(position, G1, enemy)) {
                    //printf("[DEBUG] castling squares (F1 or G1) are under attack.\n");
                    return 1;
                }
            } else { 
                if (!(position.castling_rights & WQ)) {
                    //printf("[DEBUG] missing white queen-side castling rights (WQ).\n");
                    return 1;
                }
                if (position.colors[BOTH] & ((1ULL << B1) | (1ULL << C1) | (1ULL << D1))) {
                    //printf("[DEBUG] castling path B1-C1-D1 is blocked by pieces.\n");
                    return 1;
                }
                if (is_square_attacked(position, D1, enemy) || is_square_attacked(position, C1, enemy)) {
                    //printf("[DEBUG] king path squares (D1 or C1) are under attack.\n");
                    return 1;
                }
            }
        } else { 
            if (move.castling == 0) {
                if (!(position.castling_rights & BK)) {
                    //printf("[DEBUG] missing black king-side castling rights (BK).\n");
                    return 1;
                }
                if (position.colors[BOTH] & ((1ULL << F8) | (1ULL << G8))) {
                    //printf("[DEBUG] castling path F8-G8 is blocked.\n");
                    return 1;
                }
                if (is_square_attacked(position, F8, enemy) || is_square_attacked(position, G8, enemy)) {
                    //printf("[DEBUG] black king-side squares (F8 or G8) are under attack.\n");
                    return 1;
                }
            } else {
                if (!(position.castling_rights & BQ)) {
                    //printf("[DEBUG] missing black queen-side castling rights (BQ).\n");
                    return 1;
                }
                if (position.colors[BOTH] & ((1ULL << B8) | (1ULL << C8) | (1ULL << D8))) {
                    //printf("[DEBUG] castling path B8-C8-D8 is blocked.\n");
                    return 1;
                }
                if (is_square_attacked(position, D8, enemy) || is_square_attacked(position, C8, enemy)) {
                    //printf("[DEBUG] black queen-side squares (D8 or C8) are under attack.\n");
                    return 1;
                }
            }
        }
        return 0; 
    }

    bitboard from_bit = 1ULL << move.from;
    bitboard to_bit = 1ULL << move.to;
    bitboard moves = 0ULL;

    if ((position.colors[side] & from_bit) == 0ULL) {
        //printf("[DEBUG] no piece of your color found on the source square.\n");
        return 1;
    }

    Piece moving_piece = NONE;
    for(int p = PAWN; p <= KING; p++) {
        if(position.pieces[p] & from_bit) {
            moving_piece = (Piece)p;
            break;
        }
    }

    if (moving_piece == NONE) {
        //printf("[DEBUG] error - piece not found on internal bitboards.\n");
        return 1;
    }

    switch (moving_piece) {
        case PAWN: {
            Position temp = position;
            temp.pieces[PAWN] = from_bit; 
            bitboard pushes = get_pawns_moves(temp, move.color);

            bitboard enemy_targets = position.colors[enemy];
            if ((int)position.en_passant != -1){
                enemy_targets |= (1ULL << position.en_passant);
            }

            bitboard attacks = get_pawns_attacks(temp, move.color) & enemy_targets;
            moves = (to_bit & pushes) | (to_bit & attacks);
            break;
        }
        case KNIGHT: moves = to_bit & get_knights_moves(move.from, position, move.color); break;
        case BISHOP: moves = to_bit & get_bishop_moves(move.from, position, move.color); break;
        case ROOK:   moves = to_bit & get_rook_moves(move.from, position, move.color); break;
        case QUEEN:  moves = to_bit & get_queen_moves(move.from, position, move.color); break;
        case KING:   moves = to_bit & get_kings_moves(position, move.color); break;
        default: break;
    }

    if(moves == 0ULL) {
        //printf("[DEBUG] pseudo-legality fail - piece cannot reach the target square.\n");
        return 1;
    }

    Position temp = position;
    make_move(&temp, move);
    bitboard king_bit = temp.pieces[KING] & temp.colors[side];

    if (king_bit == 0ULL) {
        //printf("[DEBUG] error - king disappeared from the board.\n");
        return 1;
    }

    Square king_sq = (Square)get_lsb_index(king_bit);

    if(is_square_attacked(temp, king_sq, enemy)) {
        //printf("[DEBUG] illegal move - king is in check after the move.\n");
        return 1;
    }

    return 0;
}

int is_square_attacked(Position position, Square sq, Color attacker){
    Color defender = (attacker == WHITE) ? BLACK : WHITE;
    
    bitboard sq_bit = 1ULL << sq; 

    bitboard attacks = position.pieces[KNIGHT] & position.colors[attacker];
    if(knight_moves[sq] & attacks) return 1;

    attacks = (position.pieces[BISHOP] | position.pieces[QUEEN]) & position.colors[attacker];
    if(attacks){
        if(get_bishop_moves(sq, position, defender) & attacks) return 1;
    }

    attacks = (position.pieces[ROOK] | position.pieces[QUEEN]) & position.colors[attacker];
    if(attacks){
        if(get_rook_moves(sq, position, defender) & attacks) return 1;
    }

    attacks = position.pieces[PAWN] & position.colors[attacker];
    if (attacker == WHITE) {
        if (((sq_bit >> 7) & ~FILE_A & attacks) || ((sq_bit >> 9) & ~FILE_H & attacks)) return 1;
    } else {
        if (((sq_bit << 7) & ~FILE_H & attacks) || ((sq_bit << 9) & ~FILE_A & attacks)) return 1;
    }

    bitboard king_attacks = 0ULL;
    king_attacks |= (sq_bit >> 8) | (sq_bit << 8);
    if (sq_bit & ~FILE_A) king_attacks |= (sq_bit << 7) | (sq_bit >> 1) | (sq_bit >> 9);
    if (sq_bit & ~FILE_H) king_attacks |= (sq_bit << 9) | (sq_bit << 1) | (sq_bit >> 7);
    
    if (king_attacks & position.pieces[KING] & position.colors[attacker]) return 1;

    return 0;
}

int has_legal_moves(Position position){
    Color side = position.whose_turn;
    Move m;
    m.color = side;
    m.piece = NONE;

    for(int from = A1; from <= H8; from++){
        if((position.colors[side] & (1ULL << from)) == 0ULL) continue;
        for(int to = A1; to <= H8; to++){
            if(from == to) continue;

            m.from = (Square) from;
            m.to = (Square) to;
            m.castling = -1;

            if(is_legal(position, m) == 0){
                return 1;
            }
        }
    }

    m.from = A1;
    m.to = A1;
    m.castling = 0;
    if(is_legal(position, m) == 0) return 1;

    m.castling = 1;
    if(is_legal(position, m) == 0) return 1;

    return 0;
}