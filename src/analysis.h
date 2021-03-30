int eval_overall(struct board boardstate);
int eval_result(struct board boardstate);
int eval_material(struct board boardstate);
int eval_position(struct board boardstate);
bool eval_check(board board_in);
piece_array eval_array_captures(board board_in, int to_file, int to_rank, bool moving);
bool _eval_array_captures(board board_in, int to_file, int to_rank, bool moving);