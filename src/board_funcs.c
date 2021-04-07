#include "structures.h"
#include "board_funcs.h"
#include "piece_funcs.h"
#include "move_funcs.h"
#include "node_funcs.h"
#include "analysis.h"

//******************************************************************
//							Board Functions							
//******************************************************************

//Given a position in fen format, returns defined board
board board_from_fen(char in[]){
	int store;
	board out;
	int string_index = 0;
	int grid_index = 0;
	
	//First, parse piece definition part of string, setting each grid square to its assigned piece
	while(in[string_index] != ' '){
		switch(in[string_index]){
			//Lowercase letters, representing black pieces
			case 'r':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(ROOK, BLACK);
				grid_index++;
				break;
			case 'n':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(KNIGHT, BLACK);
				grid_index++;
				break;
			case 'b':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(BISHOP, BLACK);
				grid_index++;
				break;
			case 'q':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(QUEEN, BLACK);
				grid_index++;
				break;
			case 'k':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(KING, BLACK);
				grid_index++;
				break;
			case 'p':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(PAWN, BLACK);
				grid_index++;
				break;
			//Uppercase letters, representing white pieces
			case 'R':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(ROOK, WHITE);
				grid_index++;
				break;
			case 'N':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(KNIGHT, WHITE);
				grid_index++;
				break;
			case 'B':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(BISHOP, WHITE);
				grid_index++;
				break;
			case 'Q':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(QUEEN, WHITE);
				grid_index++;
				break;
			case 'K':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(KING, WHITE);
				grid_index++;
				break;
			case 'P':
				out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(PAWN, WHITE);
				grid_index++;
				break;
			//Slash character
			case '/':
				break;
			//Numbers
			default:
				store = in[string_index] - 48;
				for(int i = 0; i < store; i++){
					out.grid[grid_index%8][7 - (grid_index/8)] = piece_new(VACANT, 0);
					grid_index++;
				}
				break;
		}
		string_index++;
	}
	string_index++;
	//Set current turn
	if(in[string_index] == 'w') out.to_move = WHITE;
	else out.to_move = BLACK;
	string_index += 2;
	//Parse castling rights
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
	//Set en passant tile, if it exists
	if(in[string_index] != '-'){
		out.en_passant_valid = 1;
		out.en_passant_file = in[string_index++] - 97;
		out.en_passant_rank = in[string_index++] - 48;
	}
	else{
		out.en_passant_valid = 0;
	}
	string_index += 2;
	//Set draw counter
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

//Given a board and a move, returns the board after the given move is made on the given board
//Assumes all given moves are "legal" (ignoring check); undefined behavior for illegal moves
/*struct board board_move(struct board board_in, struct move move_in){
	int from_file = move_in.from_file;
	int from_rank = move_in.from_rank;
	int to_file = move_in.to_file;
	int to_rank = move_in.to_rank;
	bool pawn_movement = board_in.grid[from_file][from_rank].type == PAWN;
	
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
	if(board_in.grid[from_file][from_rank].type == KING && (move_in.from_file - move_in.to_file != 1 && move_in.from_file - move_in.to_file != 0 && move_in.from_file - move_in.to_file != -1)){
		//Kingside castling
		if(to_file - from_file > 0){
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
	//Actually moves the piece
	board_in = _piece_move(board_in, from_file, from_rank, to_file, to_rank);
	//Flip turn bool
	board_in.to_move = !board_in.to_move;
	return board_in;
}
*/
//Given a board and a move, makes a move
//Assumes all given moves are "legal" (ignoring check); undefined behavior for illegal moves
//Also sets move_in's priority flag
board board_move(struct board board_in, struct move *move_in){
	int from_file = move_in->from_file;
	int from_rank = move_in->from_rank;
	int to_file = move_in->to_file;
	int to_rank = move_in->to_rank;
	int from_type = board_in.grid[from_file][from_rank].type;
	int to_type = board_in.grid[to_file][to_rank].type;
	int pawn_movement = from_type == PAWN;
	int priority = 0;
	
	//En passant flag handling
	if(pawn_movement && (from_rank - to_rank == 2 || from_rank - to_rank == -2)){
		board_in.en_passant_valid = true; 
		board_in.en_passant_file = to_file;
		board_in.en_passant_rank = to_rank;
	}
	else board_in.en_passant_valid = false;
	//Draw counter handling
	if(pawn_movement || to_type != VACANT) board_in.draw_counter = 0;
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
	if(from_type == KING && (from_file - to_file != 1 && from_file - to_file != 0 && from_file - to_file != -1)){
		//Kingside castling
		if(to_file - from_file > 0){
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
	else if(move_in->promote != 0){ 
		board_in.grid[from_file][from_rank].type = move_in->promote;
	}
	//En passant handling
	//The only legal diagonal pawn move that moves into an empty tile is an en passant
	else if(pawn_movement && from_file != to_file && to_type == VACANT){
		//White's move
		if(board_in.grid[from_file][from_rank].owner == WHITE){
			board_in.grid[to_file][to_rank-1].type = VACANT;
		}
		//Black's move
		else{
			board_in.grid[to_file][to_rank+1].type = VACANT;
		}
	}
/*15: promote queen
	14: capture queen with pawn/king
	13: capture rook with pawn/king
	12: capture bishop/knight with pawn/king
	11: capture piece with weaker piece (from_type < to_type) (considers bishops > knights)
	10: capture piece with equal piece (from_type == to_type)
	9: castle
	8: move queen
	7: move rook
	6: move bishop
	5: move knight
	4: capture piece with stronger piece
	3: move pawn
	2: promote to non queen*/
	if(move_in->promote == QUEEN) 																						priority = 15;
	else if((from_type == KING || from_type == PAWN) && to_type == QUEEN) 												priority = 14;
	else if((from_type == KING || from_type == PAWN) && to_type == ROOK)												priority = 13;
	else if((from_type == KING || from_type == PAWN) && (to_type == BISHOP || to_type == KNIGHT))						priority = 12;
	else if(from_type < to_type)																						priority = 11;
	else if(from_type == to_type)																						priority = 10;
	else if(from_type == KING && (from_file - to_file != 1 && from_file - to_file != 0 && from_file - to_file != -1))	priority = 9;
	else if(from_type == QUEEN)																							priority = 8;
	else if(from_type == ROOK)																							priority = 7;
	else if(from_type == BISHOP)																						priority = 6;
	else if(from_type == KNIGHT)																						priority = 5;
	else if(from_type > to_type)																						priority = 4;
	else if(from_type == PAWN)																							priority = 3;
	move_in->priority = priority;
	board_in = _piece_move(board_in, from_file, from_rank, to_file, to_rank);
	board_in.to_move = !board_in.to_move;
	return board_in;
}

//Returns a board_array of all possible legal positions reachable by one move from given position
//Final entry in array marked by invalid board (draw_counter = 127)
node_array board_legal_states(struct board board_in, int depth){
	int temp_index, pre_check_index;
	node pre_check[128];
	node_array out;
	move* temp;
	pre_check_index = 0;
	for(int file = 0; file <= 7; file++){
		for(int rank = 0; rank <= 7; rank++){
			if(board_in.grid[file][rank].owner == board_in.to_move && board_in.grid[file][rank].type != 0){
				temp = board_piece_possible_moves(board_in, file, rank);
				temp_index = 0;
				while(temp[temp_index].promote != 7){
					pre_check[pre_check_index++] = node_move(board_in, temp[temp_index++], depth);
				}
			}
		}
	}
	pre_check[pre_check_index] = node_new(board_invalid(), 0);
	pre_check_index = 0;
	temp_index = 0;
	//Removes all moves which put the moving player into check
	while(pre_check[pre_check_index].sboard.draw_counter != 127){
		pre_check[pre_check_index].sboard.to_move = !pre_check[pre_check_index].sboard.to_move;
		if(!eval_check(pre_check[pre_check_index].sboard)){
			pre_check[pre_check_index].sboard.to_move = !pre_check[pre_check_index].sboard.to_move;
			out.array[temp_index++] = pre_check[pre_check_index];
		}
		pre_check_index++;
	}
	out.array[temp_index] = node_new(board_invalid(), depth);
	return out;
}

//Returns a move array of all possible "legal" (not considering check) moves from given position, by piece at given location
//Assumes that given piece is actually able to move this turn (does not respect board_in.to_move, except for pawn movement direction)
//Final entry in array marked by invalid move (promote = 7)
move* board_piece_possible_moves(struct board board_in, int from_file, int from_rank){
	int out_index, temp_file, temp_rank;
	bool not_top, not_bottom, not_right, not_left;
	static move out[64];
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
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, QUEEN);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, ROOK);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, BISHOP);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, KNIGHT);
					}else{
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, 0);
						//Double move
						if(from_rank == 1 && board_in.grid[from_file][from_rank+2].type == VACANT) out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+2, 0);
					}
				}
				//Captures
				//Rightward captures
				if(not_right){
					//Non-promoting
					if(not_top){
						//If up-right is caputurable, or en passant target
						if(board_capturable(board_in, from_file+1, from_rank+1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file+1 && board_in.en_passant_rank == from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file+1, from_rank+1)){
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, QUEEN);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, ROOK);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, BISHOP);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, KNIGHT);
						}
					}
				}
				//Leftward captures
				if(not_left){
					//Non-promoting
					if(not_top){
						//If up-left is caputurable, or en passant target
						if(board_capturable(board_in, from_file-1, from_rank+1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file-1 && board_in.en_passant_rank == from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file-1, from_rank+1)){
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, QUEEN);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, ROOK);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, BISHOP);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, KNIGHT);
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
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, QUEEN);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, ROOK);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, BISHOP);
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, KNIGHT);
					}else{
						out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, 0);
						//Double move
						if(from_rank == 6 && board_in.grid[from_file][from_rank-2].type == VACANT) out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-2, 0);
					}
				}
				//Captures
				//Rightward captures
				if(not_right){
					//Non-promoting
					if(not_bottom){
						//If up-right is caputurable, or en passant target
						if(board_capturable(board_in, from_file+1, from_rank-1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file+1 && board_in.en_passant_rank == from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file+1, from_rank-1)){
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, QUEEN);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, ROOK);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, BISHOP);
							out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, KNIGHT);
						}
					}
				}
				//Leftward captures
				if(not_left){
					//Non-promoting
					if(not_bottom){
						//If up-left is caputurable, or en passant target
						if(board_capturable(board_in, from_file-1, from_rank-1) || (board_in.en_passant_valid && board_in.en_passant_file == from_file-1 && board_in.en_passant_rank == from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, 0);
					}
					//Promoting
					else{
						if(board_capturable(board_in, from_file-1, from_rank-1)){
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, QUEEN);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, ROOK);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, BISHOP);
							out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, KNIGHT);
						}
					}
				}
			}
			break;
		case KING:
			not_top = from_rank < 7;
			not_bottom = from_rank > 0;
			not_right = from_file < 7;
			not_left = from_file > 0;
			//Up
			if(not_top){
				//Up-Left
				if(not_left && board_moveable(board_in, from_file-1, from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+1, 0);
				//Up
				if(board_moveable(board_in, from_file, from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file, from_rank+1, 0);
				//Up-Right
				if(not_right && board_moveable(board_in, from_file+1, from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+1, 0);
			}
			//Down
			if(not_bottom){
				//Down-Left
				if(not_left && board_moveable(board_in, from_file-1, from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-1, 0);
				//Down
				if(board_moveable(board_in, from_file, from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file, from_rank-1, 0);
				//Down-Right
				if(not_right && board_moveable(board_in, from_file+1, from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-1, 0);
			}
			//Right
			if(not_right && board_moveable(board_in, from_file+1, from_rank)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank, 0);
			//Left
			if(not_left && board_moveable(board_in, from_file-1, from_rank)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank, 0);
			//Castling
			//Kingside:
			if((board_in.to_move && board_in.white_kingside) || (!board_in.to_move && board_in.black_kingside)){
				if((!_eval_array_captures(board_in, from_file, from_rank, !board_in.to_move)) && (!_eval_array_captures(board_in, from_file+1, from_rank, !board_in.to_move)) &&
					(!_eval_array_captures(board_in, from_file+2, from_rank, !board_in.to_move)) &&
					board_in.grid[from_file+1][from_rank].type == VACANT && board_in.grid[from_file+2][from_rank].type == VACANT)
					out[out_index++] = move_new(from_file, from_rank, from_file+2, from_rank, 0);
			}
			//Queenside:
			if((board_in.to_move && board_in.white_queenside) || (!board_in.to_move && board_in.black_queenside)){
				if(((!_eval_array_captures(board_in, from_file, from_rank, !board_in.to_move)) && (!_eval_array_captures(board_in, from_file-1, from_rank, !board_in.to_move)) && (!_eval_array_captures(board_in, from_file-2, from_rank, !board_in.to_move))) &&
					board_in.grid[from_file-1][from_rank].type == VACANT && board_in.grid[from_file-2][from_rank].type == VACANT && board_in.grid[from_file-3][from_rank].type == VACANT) 
					out[out_index++] = move_new(from_file, from_rank, from_file+2, from_rank, 0);
			}
			break;
		case QUEEN:
			//Vertical
			//Up
			temp_rank = from_rank+1;
			while(temp_rank <= 7){
				if(board_empty(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
					break;
				}
				temp_rank++;
			}
			//Down
			temp_rank = from_rank-1;
			while(temp_rank >= 0){
				if(board_empty(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
					break;
				}
				temp_rank--;
			}
			//Horizontal
			//Right
			temp_file = from_file+1;
			while(temp_file <= 7){
				if(board_empty(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
					break;
				}
				temp_file++;
			}
			//Left
			temp_file = from_file-1;
			while(temp_file >= 0){
				if(board_empty(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
					break;
				}
				temp_file--;
			}
			//Diagonal
			//Up-Right
			temp_file = from_file+1;
			temp_rank = from_rank+1;
			while(temp_file <= 7 && temp_rank <= 7){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file++;
				temp_rank++;
			}
			//Up-Left
			temp_file = from_file-1;
			temp_rank = from_rank+1;
			while(temp_file >= 0 && temp_rank <= 7){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file--;
				temp_rank++;
			}
			//Down-Left
			temp_file = from_file-1;
			temp_rank = from_rank-1;
			while(temp_file >= 0 && temp_rank >= 0){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file--;
				temp_rank--;
			}
			//Down-Right
			temp_file = from_file+1;
			temp_rank = from_rank-1;
			while(temp_file <= 7 && temp_rank >= 0){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file++;
				temp_rank--;
			}
			break;
		case ROOK:
			//Up
			temp_rank = from_rank+1;
			while(temp_rank <= 7){
				if(board_empty(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
					break;
				}
				temp_rank++;
			}
			//Down
			temp_rank = from_rank-1;
			while(temp_rank >= 0){
				if(board_empty(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, from_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, from_file, temp_rank, 0);
					break;
				}
				temp_rank--;
			}
			//Horizontal
			//Right
			temp_file = from_file+1;
			while(temp_file <= 7){
				if(board_empty(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
					break;
				}
				temp_file++;
			}
			//Left
			temp_file = from_file-1;
			while(temp_file >= 0){
				if(board_empty(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, from_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, from_rank, 0);
					break;
				}
				temp_file--;
			}
			break;
		case BISHOP:
			//Up-Right
			temp_file = from_file+1;
			temp_rank = from_rank+1;
			while(temp_file <= 7 && temp_rank <= 7){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file++;
				temp_rank++;
			}
			//Up-Left
			temp_file = from_file-1;
			temp_rank = from_rank+1;
			while(temp_file >= 0 && temp_rank <= 7){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file--;
				temp_rank++;
			}
			//Down-Left
			temp_file = from_file-1;
			temp_rank = from_rank-1;
			while(temp_file >= 0 && temp_rank >= 0){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file--;
				temp_rank--;
			}
			//Down-Right
			temp_file = from_file+1;
			temp_rank = from_rank-1;
			while(temp_file <= 7 && temp_rank >= 0){
				if(board_empty(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
				else{
					if(board_capturable(board_in, temp_file, temp_rank)) out[out_index++] = move_new(from_file, from_rank, temp_file, temp_rank, 0);
					break;
				}
				temp_file++;
				temp_rank--;
			}
			break;
		case KNIGHT:
			//Counting clockwise:
			//1
			if(from_file+1 <= 7 && from_rank+2 <= 7 && board_moveable(board_in, from_file+1, from_rank+2)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank+2, 0);
			//2
			if(from_file+2 <= 7 && from_rank+1 <= 7 && board_moveable(board_in, from_file+2, from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file+2, from_rank+1, 0);
			//3
			if(from_file+2 <= 7 && from_rank-1 >= 0 && board_moveable(board_in, from_file+2, from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file+2, from_rank-1, 0);
			//4
			if(from_file+1 <= 7 && from_rank-2 >= 0 && board_moveable(board_in, from_file+1, from_rank-2)) out[out_index++] = move_new(from_file, from_rank, from_file+1, from_rank-2, 0);
			//5
			if(from_file-1 >= 0 && from_rank-2 >= 0 && board_moveable(board_in, from_file-1, from_rank-2)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank-2, 0);
			//6
			if(from_file-2 >= 0 && from_rank-1 >= 0 && board_moveable(board_in, from_file-2, from_rank-1)) out[out_index++] = move_new(from_file, from_rank, from_file-2, from_rank-1, 0);
			//7
			if(from_file-2 >= 0 && from_rank+1 <= 7 && board_moveable(board_in, from_file-2, from_rank+1)) out[out_index++] = move_new(from_file, from_rank, from_file-2, from_rank+1, 0);
			//8
			if(from_file-1 >= 0 && from_rank+2 <= 7 && board_moveable(board_in, from_file-1, from_rank+2)) out[out_index++] = move_new(from_file, from_rank, from_file-1, from_rank+2, 0);
			break;
	}
	out[out_index] = move_invalid();
	return out;
}

//Returns whether or not the given position is vacant
bool board_empty(board board_in, int to_file, int to_rank){
	return board_in.grid[to_file][to_rank].type == VACANT;
}

//Returns whether or not the given tile contains a piece not owned by the moving player
bool board_capturable(board board_in, int to_file, int to_rank){
	return board_in.grid[to_file][to_rank].owner != board_in.to_move && board_in.grid[to_file][to_rank].type != VACANT;
}

//Returns whether or not the tile contains a piece owned by the non-moving player, or is empty
bool board_moveable(board board_in, int to_file, int to_rank){
	return board_in.grid[to_file][to_rank].owner != board_in.to_move || board_in.grid[to_file][to_rank].type == VACANT;
}
