#ifndef MOVE_H
#define MOVE_H

#include "board.h"

typedef struct move {
    int start;
    int end;
} *move;

move create_move();
void init_move(move m, int start, int end);
move copy_move(move m);
bool cmp_move(move m1, move m2);
void destroy_move(move m);

#endif
