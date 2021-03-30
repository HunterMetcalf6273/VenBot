#include "structures.h"

node node_new(board board_in, int depth);
node node_move(board board_in, move move_in, int depth);
int node_eval(node node_in, int max_depth, int alpha, int beta);
move node_best_move(node node_in, int max_depth, int alpha, int beta);