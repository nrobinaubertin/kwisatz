#include <stdlib.h>

#include "evaluate.h"

// Piece-Square Tables

const int eval_pawn[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0,  5, 10, 10,-20,-20, 10, 10,  5, 0,
    0,  5, -5,-10,  0,  0,-10, -5,  5, 0,
    0,  0,  0,  0, 20, 20,  0,  0,  0, 0,
    0,  5,  5, 10, 25, 25, 10,  5,  5, 0,
    0, 10, 10, 20, 30, 30, 20, 10, 10, 0,
    0, 50, 50, 50, 50, 50, 50, 50, 50, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_knight[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-50,-40,-30,-30,-30,-30,-40,-50,0,
    0,-40,-20,  0,  5,  5,  0,-20,-40,0,
    0,-30,  5, 10, 15, 15, 10,  5,-30,0,
    0,-30,  0, 15, 20, 20, 15,  0,-30,0,
    0,-30,  5, 15, 20, 20, 15,  5,-30,0,
    0,-30,  0, 10, 15, 15, 10,  0,-30,0,
    0,-40,-20,  0,  0,  0,  0,-20,-40,0,
    0,-50,-40,-30,-30,-30,-30,-40,-50,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_bishop[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-20,-10,-10,-10,-10,-10,-10,-20,0,
    0,-10,  5,  0,  0,  0,  0,  5,-10,0,
    0,-10, 10, 10, 10, 10, 10, 10,-10,0,
    0,-10,  0, 10, 10, 10, 10,  0,-10,0,
    0,-10,  5,  5, 10, 10,  5,  5,-10,0,
    0,-10,  0,  5, 10, 10,  5,  0,-10,0,
    0,-10,  0,  0,  0,  0,  0,  0,-10,0,
    0,-20,-10,-10,-10,-10,-10,-10,-20,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_rook[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
    0,  5, 10, 10, 10, 10, 10, 10,  5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0, -5,  0,  0,  0,  0,  0,  0, -5, 0,
    0,  0,  0,  0,  5,  5,  0,  0,  0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const int eval_queen[120] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,-20,-10,-10, -5, -5,-10,-10,-20,0,
    0,-10,  0,  5,  0,  0,  0,  0,-10,0,
    0,-10,  5,  5,  5,  5,  5,  0,-10,0,
    0,  0,  0,  5,  5,  5,  5,  0, -5,0,
    0, -5,  0,  5,  5,  5,  5,  0, -5,0,
    0,-10,  0,  5,  5,  5,  5,  0,-10,0,
    0,-10,  0,  0,  0,  0,  0,  0,-10,0,
    0,-20,-10,-10, -5, -5,-10,-10,-20,0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// positive score = better for white
// negative score = better for black
int evaluate(board b) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int k = 21 + j + 10*i; // index of the current square
            int kk = 21 - j + 70 - 10*i; // inverted k (for black piece square value tables)
            int kc = b->color[k]; // square color
            int kv = (kc == 1) ? k : kk; // k or kk depending on kc
            switch (b->piece[k]) {
                case 1:
                    // doubled pawns
                    if (b->piece[k + 10 * kc] == 1 && b->color[k + 10 * kc] == kc) {
                        score += kc * 50;
                    } else {
                        score += kc * 100;
                    }
                    score += kc * eval_pawn[kv];
                    break;
                case 2:
                    score += kc * (eval_knight[kv] + 300);
                    break;
                case 3:
                    score += kc * (eval_bishop[kv] + 325);
                    break;
                case 4:
                    score += kc * (eval_rook[kv] + 500);
                    break;
                case 5:
                    score += kc * (eval_queen[kv] + 900);
                    break;
                case 6:
                    score += kc * 100000;
                    break;
                default:
                    break;
            }
        }
    }
    return score * b->who;
}
