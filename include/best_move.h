#include "evaluate.h"
#include "zobrist.h"
#include "print.h"
#include "board.h"
#include "move.h"
#include "move_list.h"
#include "move_generation.h"
#include "util.h"
#include <pthread.h>
#include <math.h>
#include <stdio.h>

typedef struct arg_t {
    board b;
    int f;
    int depth;
    int qdepth;
    z_hashtable z_ht;
} *arg_t;

arg_t create_arg_t(board b, int f, int depth, int qdepth, z_hashtable z_ht);
int is_game_over(board b);
int search(board b, int depth, int alpha, int beta, int qdepth, z_hashtable z_ht);
void* MTDF(void* input);
move best_move(board b, int depth, int threads, int qdepth, z_hashtable z_ht);
