#include "structures.h"

node node_new(board board_in, int depth);
node node_move(board board_in, move move_in, int depth);
void node_swap(node *pos1, node *pos2);
void node_array_sort(node_array *array, int length);
int node_array_length(node_array *array);
move node_best_move(node node_in, int max_depth, int alpha, int beta);
int node_eval(node node_in, int max_depth, int alpha, int beta);