#include <string.h>
#include <ctype.h>
#include "move.h"
#include "constants.h"

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
    bitboard occupancy = all_pieces & bishop_masks[sq];
    
    int magic_index = (occupancy * bishop_magic_numbers[sq]) >> (64 - bishop_bits_numbers[sq]);
    bitboard raw = bishop_attacks[sq][magic_index];

    return raw & ~position.colors[color];
}

bitboard get_rook_moves(Square sq, Position position, Color color){
    bitboard all_pieces = position.colors[BOTH];
    bitboard occupancy = all_pieces & rook_masks[sq];

    int magic_index = (occupancy * rook_magic_numbers[sq]) >> (64 - rook_bits_numbers[sq]);
    bitboard raw = rook_attacks[sq][magic_index];

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
        return -1;
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
           to_file < 0 || to_file > 7 || to_rank < 0 || to_rank > 7) return -1;

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
        return -1;
    }

    return 1;
}

int make_move(Position *position, Move move) {
    bitboard from_bit = 1ULL << move.from;
    bitboard to_bit = 1ULL << move.to;

    Color side = position->whose_turn;
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    Piece moving_piece = NONE;
    for(int p = PAWN; p <= KING; p++){
        if(position->pieces[p] & from_bit){
            moving_piece = p;
            break;
        }
    }

    if (moving_piece == NONE) return -1;

    position->pieces[moving_piece] &= ~from_bit;
    position->colors[side] &= ~from_bit;

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

    //todo: castling moves
    //todo: en passant

    return 1;
}

int is_legal(Position position, Move move){
    bitboard from_bit = 1ULL << move.from;
    bitboard to_bit = 1ULL << move.to;
    bitboard moves = 0ULL;

    Color side = position.whose_turn;
    Color enemy = (side == WHITE) ? BLACK : WHITE;

    if ((position.colors[side] & from_bit) == 0ULL) {
        return -1;
    }

    Piece moving_piece = NONE;
    for(int p = PAWN; p <= KING; p++){
        if(position.pieces[p] & from_bit){
            moving_piece = p;
            break;
        }
    }

    if (moving_piece == NONE) return -1;

    switch (moving_piece){
        case PAWN: {
            Position temp = position;
            temp.pieces[PAWN] = from_bit; 

            bitboard pushes = get_pawns_moves(temp, move.color);
            bitboard attacks = get_pawns_attacks(temp, move.color) & position.colors[enemy];
            
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

    if(moves == 0ULL) return -1;

    return 1;
}