#include "move.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A move list a an array of 100 moves max.
// A move is a start (int) and an end (int)

move create_move() {
    return calloc(1, sizeof(struct move));
}

void init_move(move m, int start, int end) {
    m->start = start;
    m->end = end;
}

move copy_move(move m) {
    if (m == NULL) {
        return NULL;
    }
    move mm = calloc(1, sizeof(struct move));
    mm->start = m->start;
    mm->end = m->end;
    return mm;
}

bool cmp_move(move m1, move m2) {
    if (NULL == m1 || NULL == m2) {
        return false;
    }
    return m1->start == m2->start && m1->end == m2->end;
}

void destroy_move(move m) {
    free(m);
}
