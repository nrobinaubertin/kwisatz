#include "board.h"
#include "evaluate.h"
#include "move_generation.h"
#include "print.h"
#include "best_move.h"
#include "board.h"
#include "move.h"
#include "move_list.h"
#include "zobrist.h"
#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// zobrist hashtable
z_hashtable z_ht = NULL;

// perft statistics
int checks = 0;
int captures = 0;
int castles = 0;
int checkmates = 0;

// perft test: recursively count all move nodes and checks
// compare results with https://chessprogramming.org/Perft_Results
// obviously, since this engine doesn't take "en passant" into account,
// values are going to differ starting depth 5
int perft(board b, int depth, int last_move_type) {

    // count various statistics for leaf nodes
    if (depth == 0) {
        if (is_king_checked(b, b->who)) {
            checks++;
        }
        if (is_checkmate(b)) {
            checkmates++;
        }
        switch (last_move_type) {
            case 2:
                castles++;
                break;
            case 1:
                captures++;
                break;
            default:
                break;
        }
        return 1;
    }

    int nodes = 0;
    move_list ml = gen_all_moves(b, false);
    for (int i = 0; i < ml->size; i++) {
        board bb = copy_board(b);
        last_move_type = apply_move(ml->list[i], bb);
        // rule out moves that keep the king in check
        if (!is_king_checked(bb, bb->who*-1)) {
            nodes += perft(bb, depth - 1, last_move_type);
        }
        destroy_board(bb);
    }
    destroy_move_list(ml);
    return nodes;
}

// this will make the AI play against itself
// depth: number of plies to look forward in alpha-beta
// duration: number of plies to self-play
// debug: should we display more informations ?
// threads: number of threads to use
// qdepth: depth of quiescence search (added to depth in alpha-beta)
void play_alone(int depth, int duration, bool debug, int threads, int qdepth, int seed) {
    // prepare the zobrist hashtable
    // max size: 2_000_000_000
    // because parameters are passed as int ?
    // static numbers are ints ?
    z_ht = create_hashtable(100000000, seed);

    board b = create_board();
    init_board(b);
    if (debug) {
        print_board(b);
        printf("\n");
    }

    int w = 0;
    int turn = 0;
    while (!(w = is_game_over(b)) && turn < duration) {
        move m = best_move(b, depth, threads, qdepth, z_ht);

        if (!m) {
            if (b->who == 1) {
                printf("White resigns !\n");
            } else {
                printf("Black resigns !\n");
            }
            printf("\n");
            print_board(b);
            printf("\n");
            destroy_board(b);
            return;
        }

        print_move(m, debug);
        apply_move(m, b);
        if (debug) {
            printf("\n");
            print_board(b);
            printf("\n");
        }
        free(m);
        turn++;
    }

    if (w == 1) {
        printf("White wins !\n");
    } else if (w == -1) {
        printf("Black wins !\n");
    } else {
        printf("Draw !\n");
    }
    destroy_board(b);
    destroy_hashtable(z_ht);
}

// execute perft test
int execute_perft(int depth) {

    board b = create_board();
    init_board(b);
    int nodes = perft(b, depth, 0);
    printf("nodes: %d\n", nodes);
    printf("checks: %d\n", checks);
    printf("castles: %d\n", castles);
    printf("captures: %d\n", captures);
    printf("checkmates: %d\n", checkmates);
    destroy_board(b);
    return nodes;
}

// prompt human for next move
move askForMove() {
    char start[2];
    char end[2];
    printf("move: ");
    scanf(" %c%c%c%c", &start[0], &start[1], &end[0], &end[1]);
    move m = create_move();
    m->start = coord2int(start);
    m->end = coord2int(end);
    return m;
}

// play against human
void play(int color, int depth, int threads, int qdepth) {
    board b = create_board();
    init_board(b);
    print_board(b);

    int w = 0;
    move m = NULL;
    while (!(w = is_game_over(b))) {
        if (b->who == color) {
            m = best_move(b, depth, threads, qdepth, NULL);
            if (!m) {
                if (b->who == 1) {
                    printf("White resigns !\n");
                } else {
                    printf("Black resigns !\n");
                }
                printf("\n");
                print_board(b);
                printf("\n");
                destroy_board(b);
                return;
            }
        } else {
            m = askForMove();
        }
        printf("\n");
        print_move(m, false);
        printf("\n");
        apply_move(m, b);
        print_board(b);
        free(m);
    }

    if (w == 1) {
        printf("White wins !\n");
    } else if (w == -1) {
        printf("Black wins !\n");
    } else {
        printf("Draw !\n");
    }
}

int main(int argc, char* argv[]) {
    if (!argv[1]) {
        printf("No argument !\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "perft") == 0) {
        if (!argv[2]) {
            printf("No depth !\n");
            return EXIT_FAILURE;
        }

        execute_perft(atoi(argv[2]));

    } else if(strcmp(argv[1], "play") == 0) {
        if (argc < 3) {
            printf("chess play <depth> <threads>\n");
            return EXIT_FAILURE;
        }
        int depth = atoi(argv[2]);
        int threads = atoi(argv[3]);
        play(-1, depth, threads, depth);
    } else if(strcmp(argv[1], "alone") == 0) {
        if (argc < 4) {
            printf("chess alone <depth> <duration> <threads> <qdepth>\n");
            return EXIT_FAILURE;
        }
        int depth = atoi(argv[2]);
        int duration = atoi(argv[3]);
        int threads = atoi(argv[4]);
        int qdepth = atoi(argv[5]);
        play_alone(depth, duration, false, threads, qdepth, 0);
    } else {
        printf("Unknown command.\n");
    }
    return EXIT_SUCCESS;
}
