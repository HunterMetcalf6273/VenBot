#include "structures.h"
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
//TODO: Implement
/*board board_from_fen(char in[]){
	int file, rank, index;
	board out;
	index = 0;
	while(in[index] != '\0'){
		
	}
}*/

//Given a move-chain and board, makes the given moves on the given board
//and returns the final board
//board board_from_moves(move cur,board boardstate){
//}

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
	//Promotion handling
	if(move_in.promote != 0){ 
		board_in.grid[to_file][to_rank].type = move_in.promote;
	}
	board_in.to_move = !board_in.to_move;
	return board_in;
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