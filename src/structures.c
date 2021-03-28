#include <stdio.h>
#include "structures.h"
#include "analysis.h"
//******************************************************************
//							Board Functions							
//******************************************************************

//Creates a board representing the initial Chess setup
board board_new(){
	board out;
	out.to_move = WHITE;
	out.draw_counter = 0;
	out.grid[0][1] = new_piece(PAWN, WHITE);
	out.grid[1][1] = new_piece(PAWN, WHITE);
	out.grid[2][1] = new_piece(PAWN, WHITE);
	out.grid[3][1] = new_piece(PAWN, WHITE);
	out.grid[4][1] = new_piece(PAWN, WHITE);
	out.grid[5][1] = new_piece(PAWN, WHITE);
	out.grid[6][1] = new_piece(PAWN, WHITE);
	out.grid[7][1] = new_piece(PAWN, WHITE);
	out.grid[0][6] = new_piece(PAWN, BLACK);
	out.grid[1][6] = new_piece(PAWN, BLACK);
	out.grid[2][6] = new_piece(PAWN, BLACK);
	out.grid[3][6] = new_piece(PAWN, BLACK);
	out.grid[4][6] = new_piece(PAWN, BLACK);
	out.grid[5][6] = new_piece(PAWN, BLACK);
	out.grid[6][6] = new_piece(PAWN, BLACK);
	out.grid[7][6] = new_piece(PAWN, BLACK);
	out.grid[0][0] = new_piece(ROOK, WHITE);
	out.grid[1][0] = new_piece(KNIGHT, WHITE);
	out.grid[2][0] = new_piece(BISHOP, WHITE);
	out.grid[3][0] = new_piece(QUEEN, WHITE);
	out.grid[4][0] = new_piece(KING, WHITE);
	out.grid[5][0] = new_piece(BISHOP, WHITE);
	out.grid[6][0] = new_piece(KNIGHT, WHITE);
	out.grid[7][0] = new_piece(ROOK, WHITE);
	out.grid[0][7] = new_piece(ROOK, BLACK);
	out.grid[1][7] = new_piece(KNIGHT, BLACK);
	out.grid[2][7] = new_piece(BISHOP, BLACK);
	out.grid[3][7] = new_piece(QUEEN, BLACK);
	out.grid[4][7] = new_piece(KING, BLACK);
	out.grid[5][7] = new_piece(BISHOP, BLACK);
	out.grid[6][7] = new_piece(KNIGHT, BLACK);
	out.grid[7][7] = new_piece(ROOK, BLACK);
	for(int i = 0; i <= 7; i++){
		for(int j = 2; j <= 5; j++){
			out.grid[i][j] = new_piece(VACANT, 0);
		}
	}
	return out;
}

//Given a position in fen format, returns defined board
//TODO: Document & test
board board_from_fen(char in[]){
	int file, rank, string_index, grid_index, store;
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
				string_index += store - 1;
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
	}
	string_index++;
	if(in[string_index] != '-'){
		out.en_passant_valid = 1;
		out.en_passant_file = in[string_index++] - 97;
		out.en_passant_rank = in[string_index++] - 48;
	}
	string_index += 2;
	out.draw_counter = in[string_index] - 48;
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
//Assumes all given moves are legal; undefined behavior for illegal moves
//TODO: Add counter/flag handling
struct board board_move(struct board board_in, struct move move_in){
	int from_file, from_rank, to_file, to_rank;
	from_file = move_in.from_file;
	from_rank = move_in.from_rank;
	to_file = move_in.to_file;
	to_rank = move_in.to_rank;
	//Castling handling
	//The only legal move where the king moves more than one tile is the castle, so if the difference in file is > 1, then it must be a castle
	if(board_in.grid[from_file][from_rank].type == KING && (move_in.from_file - move_in.to_file != 1 && move_in.to_file - move_in.to_rank != -1)){
		//Kingside castling
		if(from_file - to_file > 0){
			//Moves rook; king is moved at end of function
			board_in = _piece_move(board_in, 7, from_rank, 5, to_rank);
		}
		//Queenside castling
		else{
			//Same as above
			board_in = _piece_move(board_in, 0, from_rank, 2, to_rank);
		}
	}
	//Promotion handling
	else if(move_in.promote != 0){ 
		board_in.grid[from_file][from_rank].type = move_in.promote;
	}
	//En passant handling
	//The only legal diagonal pawn move that moves into an empty tile is an en passant
	else if(board_in.grid[from_file][from_rank].type == PAWN && from_file != to_file && board_in.grid[to_file][to_rank].type == VACANT){
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
//Returns a linked list of all legal moves by the given piece, from the given position
//TODO: Implement
move_node* board_get_legal_moves(board in, int from_file, int from_rank){
	
}


//Returns a linked list of move_nodes for every possible legal move from the board position given by in
//TODO: Implement
move_node* board_get_legal_move_list(board in){
	
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

move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote){
	move out;
	out.from_file = from_file;
	out.from_rank = from_rank;
	out.to_file = to_file;
	out.to_rank = to_rank;
	out.promote = promote;
	return out;
}

//******************************************************************
//							board_node Functions						
//******************************************************************

//Creates a board_node from given parameters (makes move move_in on board board_in; does NOT copy board_in to board)
//Only intended to be called by eval_board_node
board_node _board_node_new(board board_in, move move_in, int depth){
	board_node out;
	out.cur_board = board_move(board_in, move_in);
	out.depth = depth;
	return out;
}
//Returns the calculated value of the given node, considering all its possible children to given max depth
//TODO:Test & Document
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
}






















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