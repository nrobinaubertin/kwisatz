#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include "move.h"

typedef struct move_list {
    move* list;
    int size;
} *move_list;

move_list create_move_list();
void push_move_list(move_list ml, move m);
//move pop_move_list(move_list ml);
void destroy_move_list(move_list ml);

#endif
