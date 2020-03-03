#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

typedef struct board {
    int* color; // WHITE = 1, BLACK = -1, or EMPTY = 0
    int* piece; // NOT USED, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY
    int king_square[2]; // save the kings square for fast checks, 0 = WHITE, 1 = BLACK
    bool castling_rights[4]; // castling rights, 0,1 = WHITE, 2,3 = BLACK
    int who; // who's turn it is (color)
    uint64_t key; // hash
} *board;

board create_board();
void init_board(board b);
board copy_board(board b);
bool cmp_board(board b1, board b2);
void destroy_board(board b);

//uint64_t hash_piece(int color, int piece, int k);
uint64_t hash_board(board b, uint64_t* hashpool); // gets the hash of the board

#endif
