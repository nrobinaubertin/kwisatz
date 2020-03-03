#ifndef PRINT_H
#define PRINT_H

#include "util.h"
#include "board.h"
#include "move.h"

void print_square(int color, int piece);
void print_board(board b);
void print_int2coord(int k);
void print_move(move m, bool debug);

#endif
