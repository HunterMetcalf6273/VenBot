#include <stdio.h>
#include "structures.h"
#include "analysis.h"
//******************************************************************
//							Board Functions							
//******************************************************************

//Given a position in fen format, returns defined board
//TODO: Document & test
board board_from_fen(char in[]){
	int string_index, grid_index, store;
	board out;
	string_index = 0;
	grid_index = 0;
	while(in[string_index] != ' '){
		switch(in[string_index]){
			//Lowercase letters, representing black pieces
			case 'r':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(ROOK, BLACK);
				grid_index++;
				break;
			case 'n':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(KNIGHT, BLACK);
				grid_index++;
				break;
			case 'b':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(BISHOP, BLACK);
				grid_index++;
				break;
			case 'q':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(QUEEN, BLACK);
				grid_index++;
				break;
			case 'k':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(KING, BLACK);
				grid_index++;
				break;
			case 'p':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(PAWN, BLACK);
				grid_index++;
				break;
			//Uppercase letters, representing white pieces
			case 'R':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(ROOK, WHITE);
				grid_index++;
				break;
			case 'N':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(KNIGHT, WHITE);
				grid_index++;
				break;
			case 'B':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(BISHOP, WHITE);
				grid_index++;
				break;
			case 'Q':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(QUEEN, WHITE);
				grid_index++;
				break;
			case 'K':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(KING, WHITE);
				grid_index++;
				break;
			case 'P':
				out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(PAWN, WHITE);
				grid_index++;
				break;
			//Slash character
			case '/':
				break;
			//Numbers
			default:
				store = in[string_index] - 48;
				for(int i = 0; i < store; i++){
					out.grid[grid_index%8][7 - (grid_index/8)] = new_piece(VACANT, 0);
					grid_index++;
				}
				break;
		}
		string_index++;
	}
	string_index++;
	if(in[string_index] == 'w') out.to_move = WHITE;
	else out.to_move = BLACK;
	string_index += 2;
	while(in[string_index] != ' '){
		switch(in[string_index]){
			case 'K':
				out.white_kingside = 1;
				break;
			case 'Q':
				out.white_queenside = 1;
				break;
			case 'k':
				out.black_kingside = 1;
				break;
			case 'q':
				out.black_queenside = 1;
				break;
		}
		string_index++;
	}
	string_index++;
	if(in[string_index] != '-'){
		out.en_passant_valid = 1;
		out.en_passant_file = in[string_index++] - 97;
		out.en_passant_rank = in[string_index++] - 48;
	}
	else{
		out.en_passant_valid = 0;
	}
	string_index += 2;
	out.draw_counter = in[string_index] - 48;
	return out;
}

//Returns an invalid boardstate (the game should be drawn by the time draw_counter = 75, so it can never reach 127)
board board_invalid(){
	board out;
	out.draw_counter = 127;
	return out;
}
//Moves a piece to a given location, destroying whatever
//is there and emptying the original tile
//Intended only for use by board_move
struct board _piece_move(struct board board_in, int from_file, int from_rank, int to_file, int to_rank){
	board_in.grid[to_file][to_rank] = board_in.grid[from_file][from_rank];
	board_in.grid[from_file][from_rank].type = VACANT;
	return board_in;
}

//Given a board and a move, makes a move
//Assumes all given moves are "legal" (ignoring check); undefined behavior for illegal moves
struct board board_move(struct board board_in, struct move move_in){
	int from_file, from_rank, to_file, to_rank;
	bool pawn_movement;
	from_file = move_in.from_file;
	from_rank = move_in.from_rank;
	to_file = move_in.to_file;
	to_rank = move_in.to_rank;
	pawn_movement = board_in.grid[from_file][from_rank].type == PAWN;
	//En passant flag handling
	if(pawn_movement && (from_rank - to_rank == 2 || from_rank - to_rank == -2)){
		board_in.en_passant_valid = true; 
		board_in.en_passant_file = to_file;
		board_in.en_passant_rank = to_rank;
	}
	else board_in.en_passant_valid = false;
	//Draw counter handling
	if(pawn_movement || board_in.grid[to_file][to_rank].type != VACANT) board_in.draw_counter = 0;
	else board_in.draw_counter++;
	//Castling rights handling
	//White's queenside rook
	if(board_in.white_queenside && from_file == 0 && from_rank == 0) board_in.white_queenside = false;
	//White's kingside rook
	else if(board_in.white_kingside && from_file == 7 && from_rank == 0) board_in.white_kingside = false;
	//White's king
	else if((board_in.white_queenside || board_in.white_kingside) && from_file == 4 && from_rank == 0){
		board_in.white_queenside = false;
		board_in.white_kingside = false;
	}
	//Black's queenside rook
	else if(board_in.black_queenside && from_file == 0 && from_rank == 0) board_in.black_queenside = false;
	//Black's kingside rook
	else if(board_in.black_kingside && from_file == 7 && from_rank == 0) board_in.black_kingside = false;
	//Black's king
	else if((board_in.black_queenside || board_in.black_kingside) && from_file == 4 && from_rank == 7){
		board_in.black_queenside = false;
		board_in.black_kingside = false;
	}
	//Castling handling
	//The only legal move where the king moves more than one tile is the castle, so if the difference in file is > 1, then it must be a castle
	if(board_in.grid[from_file][from_rank].type == KING && (move_in.from_file - move_in.to_file > 1 && move_in.from_file - move_in.to_file < -1)){
		//Kingside castling
		if(from_file - to_file > 0){
			//Moves rook; king is moved at end of function
			board_in = _piece_move(board_in, 7, from_rank, 5, from_rank);
		}
		//Queenside castling
		else{
			//Same as above
			board_in = _piece_move(board_in, 0, from_rank, 2, from_rank);
		}
	}
	//Promotion handling
	else if(move_in.promote != 0){ 
		board_in.grid[from_file][from_rank].type = move_in.promote;
	}
	//En passant handling
	//The only legal diagonal pawn move that moves into an empty tile is an en passant
	else if(pawn_movement && from_file != to_file && board_in.grid[to_file][to_rank].type == VACANT){
		//White's move
		if(board_in.grid[from_file][from_rank].owner == WHITE){
			board_in.grid[to_file][to_rank-1].type = VACANT;
		}
		//Black's move
		else{
			board_in.grid[to_file][to_rank+1].type = VACANT;
		}
	}
	board_in = _piece_move(board_in, from_file, from_rank, to_file, to_rank);
	board_in.to_move = !board_in.to_move;
	return board_in;
}

//Returns a board_array of all possible legal positions reachable by one move from given position
//Final entry in array marked by invalid board (draw_counter = 127)
board_array board_legal_states(struct board board_in){
	int temp_index, pre_check_index;
	board_array pre_check, out;
	move_array temp;
	pre_check_index = 0;
	for(int file = 0; file <= 7; file++){
		for(int rank = 0; rank <= 7; rank++){
			if(board_in.grid[file][rank].owner == board_in.to_move && board_in.grid[file][rank].type != 0){
				temp = board_piece_possible_moves(board_in, file, rank);
				temp_index = 0;
				while(temp.array[temp_index].promote != 7){
					pre_check.array[pre_check_index++] = board_move(board_in, temp.array[temp_index++]);
				}
			}
		}
	}
	pre_check.array[pre_check_index] = board_invalid();
	pre_check_index = 0;
	temp_index = 0;
	//Removes all moves which put the moving player into check
	while(pre_check.array[pre_check_index].draw_counter != 127){
		if(!eval_check(pre_check.array[pre_check_index])) out.array[temp_index++] = pre_check.array[pre_check_index];
		pre_check_index++;
	}
	out.array[temp_index] = board_invalid();
	return out;
}

//Returns a move_array of all possible "legal" (not considering check) moves from given position, by piece at given location
//Assumes that given piece is actually able to move this turn (does not respect board_in.to_move, except for pawn movement direction)
//Final entry in array marked by invalid move (promote = 7)
//TODO: Implement
move_array board_piece_possible_moves(struct board board_in, int from_file, int from_rank){
	int out_index;
	bool temp_file, temp_rank, not_top, not_bottom, not_right, not_left;
	move_array out;
	out_index = 0;
	switch(board_in.grid[from_file][from_rank].type){
		case PAWN:
			not_right = from_file < 7;
			not_left = from_file > 0;
			//White's pawn
			if(board_in.to_move == WHITE){
				not_top = from_rank < 6;
				//Forward movement
				if(board_in.grid[from_file][from_rank+1].type == VACANT){
					//If reaching enemy backrank, promote
					if(!not_top){
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, QUEEN);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, ROOK);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, BISHOP);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, KNIGHT);
					}else{
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, 0);
						//Double move
						if(from_rank == 1 && board_in.grid[from_file][from_rank+2].type == VACANT) move_new(from_file, from_rank, from_file, from_rank+2, 0);
					}
				}
				//Captures
				//Rightward captures
				if(not_right){
					//Non-promoting
					if(not_top){
						//If up-right is caputurable, or en passant target
						if(board_capturable(board_in, from_file+1, from_rank+1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file+1 && board_in.en_passant_rank == from_rank+1)) out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file+1, from_rank+1)){
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, QUEEN);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, ROOK);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, BISHOP);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, KNIGHT);
						}
					}
				}
				//Leftward captures
				if(not_left){
					//Non-promoting
					if(not_top){
						//If up-left is caputurable, or en passant target
						if(board_capturable(board_in, from_file-1, from_rank+1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file-1 && board_in.en_passant_rank == from_rank+1)) out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file-1, from_rank+1)){
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, QUEEN);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, ROOK);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, BISHOP);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, KNIGHT);
						}
					}
				}
			}
			//Black's pawn
			else{
				not_bottom = from_rank > 1;
				//Forward movement
				if(board_in.grid[from_file][from_rank-1].type == VACANT){
					//If reaching enemy backrank, promote
					if(!not_bottom){
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, QUEEN);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, ROOK);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, BISHOP);
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, KNIGHT);
					}else{
						out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, 0);
						//Double move
						if(from_rank == 6 && board_in.grid[from_file][from_rank-2].type == VACANT) move_new(from_file, from_rank, from_file, from_rank-2, 0);
					}
				}
				//Captures
				//Rightward captures
				if(not_right){
					//Non-promoting
					if(not_bottom){
						//If up-right is caputurable, or en passant target
						if(board_capturable(board_in, from_file+1, from_rank-1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file+1 && board_in.en_passant_rank == from_rank-1)) out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file+1, from_rank-1)){
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, QUEEN);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, ROOK);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, BISHOP);
							out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, KNIGHT);
						}
					}
				}
				//Leftward captures
				if(not_left){
					//Non-promoting
					if(not_bottom){
						//If up-left is caputurable, or en passant target
						if(board_capturable(board_in, from_file-1, from_rank-1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file-1 && board_in.en_passant_rank == from_rank-1)) out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file-1, from_rank-1)){
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, QUEEN);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, ROOK);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, BISHOP);
							out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, KNIGHT);
						}
					}
				}
			}
		case KING:
			not_top = from_rank < 7;
			not_bottom = from_rank > 0;
			not_right = from_file < 7;
			not_left = from_file > 0;
			//Up
			if(not_top){
				//Up-Left
				if(not_left && board_moveable(board_in, from_file-1, from_rank+1)) out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, 0);
				//Up
				if(board_moveable(board_in, from_file, from_rank+1)) out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, 0);
				//Up-Right
				if(not_right && board_moveable(board_in, from_file+1, from_rank+1)) out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, 0);
			}
			//Down
			if(not_bottom){
				//Down-Left
				if(not_left && board_moveable(board_in, from_file-1, from_rank-1)) out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, 0);
				//Down
				if(board_moveable(board_in, from_file, from_rank-1)) out.array[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, 0);
				//Down-Right
				if(not_right && board_moveable(board_in, from_file+1, from_rank-1)) out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, 0);
			}
			//Right
			if(not_right && board_moveable(board_in, from_file+1, from_rank)) out.array[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank, 0);
			//Left
			if(not_left && board_moveable(board_in, from_file-1, from_rank)) out.array[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank, 0);
			break;
		case QUEEN:
			not_top = from_rank < 7;
			not_bottom = from_rank > 0;
			not_right = from_file < 7;
			not_left = from_file > 0;
		case ROOK:
			not_top = from_rank < 7;
			not_bottom = from_rank > 0;
			not_right = from_file < 7;
			not_left = from_file > 0;
		case BISHOP:
			not_top = from_rank < 7;
			not_bottom = from_rank > 0;
			not_right = from_file < 7;
			not_left = from_file > 0;
		case KNIGHT:
			not_top = from_rank < 6;
			not_bottom = from_rank > 1;
			not_right = from_file < 6;
			not_left = from_file > 1;
			break;
	}
	out.array[out_index] = move_invalid();
	return out;
}

//Returns whether or not the given tile contains a piece not owned by the moving player
bool board_capturable(board board_in, int to_file, int to_rank){
	return board_in.grid[to_file][to_rank].owner != board_in.to_move && board_in.grid[to_file][to_rank].type != VACANT;
}

//Returns whether or not the tile contains a piece owned by the moving player, or is empty
bool board_moveable(board board_in, int to_file, int to_rank){
	return board_in.grid[to_file][to_rank].owner != board_in.to_move || board_in.grid[to_file][to_rank].type == VACANT;
}

//******************************************************************
//							Piece Functions							
//******************************************************************

//Returns a piece, given its location, type, and owner
piece new_piece(int type, int owner){
	piece out;
	out.type = type;
	out.owner = owner;
	return out;
}

//******************************************************************
//							Move Functions							
//******************************************************************

//Returns a move, as defined by parameters
move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote){
	move out;
	out.from_file = from_file;
	out.from_rank = from_rank;
	out.to_file = to_file;
	out.to_rank = to_rank;
	out.promote = promote;
	return out;
}
//Returns a move with the promote field set to 7, to indicate an undefined move
move move_invalid(){
	move out;
	out.promote = 7;
	return out;
}
//Converts a string which starts with a long algebraic chess move (which does not contain an x to mark captures),
//delimited by either a space or a null character, into a move struct
//Undefined behavior if given anything else
move move_from_string(char *str){
	move out;
	out.from_file = *(str) - 97;
	out.from_rank = *(str+1) - 97;
	out.to_file = *(str+2) - 97;
	out.to_rank = *(str+3) - 97;
	switch(*(str+4)){
		case 'q':
			out.promote = QUEEN;
			break;
		case 'r':
			out.promote = ROOK;
			break;
		case 'b':
			out.promote = BISHOP;
			break;
		case 'n':
			out.promote = KNIGHT;
			break;
		default:
			out.promote = 0;
			break;
	}
	return out;
}

//Creates a board_node from given parameters (makes move move_in on board board_in; does NOT copy board_in to board)
//Only intended to be called by eval_board_node
board_node _board_node_new(board board_in, move move_in, int depth){
	board_node out;
	out.cur_board = board_move(board_in, move_in);
	out.depth = depth;
	return out;
}
//Returns the calculated value of the given node, considering all its possible children to given max depth
/*/TODO:Test & Document
int eval_board_node(board_node node_in, int max_depth){
	int out, child_value;
	move_node* list_cur;
	//If max depth has not been reached, generate linked list of possible moves
	if(node_in.depth <= max_depth) list_cur = board_get_legal_move_list(node_in.cur_board);
	//If no possible moves or max depth reached, return calculated value of boardstate
	if(list_cur == NULL) return eval_overall(node_in.cur_board);
	//White to choose next move
	if(node_in.cur_board.to_move == BLACK){
		out = -2147483648;
		while(list_cur != NULL){
			child_value = eval_board_node(_board_node_new(node_in.cur_board, list_cur->stored_move, node_in.depth + 1), max_depth);
			if(child_value > out) out = child_value;
			list_cur = list_cur->next;
		}
	}
	//Black to choose next move
	else{
		out = 2147483647;
		while(list_cur != NULL){
			child_value = eval_board_node(_board_node_new(node_in.cur_board, list_cur->stored_move, node_in.depth + 1), max_depth);
			if(child_value < out) out = child_value;
			list_cur = list_cur->next;
		}
	}
	return out;
}*/

















/*//Recursive function which determines the value of the given node, considering all of its children
int move_node_eval(move_node* node){
	int out, left_val;
	bool has_down, has_left;
	move_node start;
	has_down = start.down != NULL;
	has_left = start.left != NULL;
	start = *node;
	//If this node has a downward node, then its value should be that node's value
	if(has_down){
		out = move_node_eval(start.down);
	}
	//If it doesn't, then its value should be calculated
	else{
		out = eval_overall(start.next_board);
	}
	//If there isn't a left node, then we are done
	//If there is, we must set our value to the value most favorable to the player making the move (because it is assumed that the move resulting in that value will be played)
	if(has_left){
		left_val = move_node_eval(start.left);
		//If this move was made by white
		if(start.next_board.to_move == BLACK){
			if(left_val > out) out = left_val;
		}
		//If this move was made by black
		else {
			if(left_val < out) out = left_val;
		}
	}
	return out;
}*/