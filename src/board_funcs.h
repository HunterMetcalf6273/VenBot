#include "structures.h"

board board_from_fen(char in[]);
board board_invalid();
struct board _piece_move(struct board board_in, int from_file, int from_rank, int to_file, int to_rank);
board board_move(struct board board_in, struct move *move_in);
node_array board_legal_states(struct board board_in, int depth);
move* board_piece_possible_moves(struct board board_in, int from_file, int from_rank);
bool board_empty(board board_in, int to_file, int to_rank);
bool board_capturable(board board_in, int to_file, int to_rank);
bool board_moveable(board board_in, int to_file, int to_rank);