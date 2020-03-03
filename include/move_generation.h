#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include "board.h"
#include "move.h"
#include "move_list.h"

//move rand_move(board b);
void add_move_to_ml(move_list ml, int start, int end);
move_list add_ray_moves(board b, move_list ml, int piece_type, int square, int color, bool quiescent);
move_list add_point_moves(board b, move_list ml, int piece_type, int square, int color, bool quiescent);
move_list add_castle_moves(board b, move_list ml, int color);
move_list gen_all_moves(board b, bool quiescent);
move_list add_move_list(board b, int square, move_list ml, int piece, int color, bool quiescent);
bool is_checkmate(board b);
bool is_king_checked(board b, int color);
bool is_square_checked(board b, int color, int square);
void atomic_move(board b, int start, int end);
void change_castling_rights(board b, int castle, bool value);
int apply_move(move m, board b);

#endif
