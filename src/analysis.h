int eval_overall(struct board boardstate);
int eval_checkmate(struct board boardstate);
int eval_material(struct board boardstate);
int eval_position(struct board boardstate);
bool eval_check(board board_in);
move_array eval_array_threats(board board_in, int to_file, int to_rank);