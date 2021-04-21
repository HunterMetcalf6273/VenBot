bool eval_check(board board_in);
piece* eval_array_captures(board board_in, int to_file, int to_rank, bool moving);
bool _eval_array_captures(board board_in, int to_file, int to_rank, bool moving);
int eval_result(struct board boardstate);
int eval_board(struct board boardstate);


