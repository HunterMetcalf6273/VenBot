#include <stdio.h>
#include "structures.h"
#include "analysis.h"

//******************************************************************
//					Helper Functions						
//******************************************************************

//Checks if the moving player is in check
bool eval_check(board board_in){
	int king_location_file, king_location_rank;
	move_array moves;
	king_location_file = 0;
	king_location_rank = 0;
	//Iterate over grid until moving player's king is found
	while(king_location_file < 8){
		while(king_location_rank < 8){
			if(board_in.grid[king_location_file][king_location_rank].type == KING && board_in.grid[king_location_file][king_location_rank].owner == board_in.to_move) break;
			king_location_rank++;
		}
		king_location_file++;
	}
	//Flip to_move bool, then check if king can be captured
	board_in.to_move = !board_in.to_move;
	moves = eval_array_threats(board_in, king_location_file, king_location_rank);
	//If the first entry doesn't contain a valid move, then there are no valid threats to the given tile, so the king is not in check
	//TODO: implement with king-specific function instead, to avoid unnecessary calculations
	return moves.array[0].promote != 7;
}
//Returns an array of all moves the moving player could make which capture the given tile
//TODO: Implement
move_array eval_array_threats(board board_in, int to_file, int to_rank){
	int temp_file, temp_rank, moves_index;
	bool moving;
	move_array array_out;
	moves_index = 0;
	moving = board_in.to_move;
	//Check for vertical attacks
	//From above
	temp_rank = to_rank+1;
	while(temp_rank++ <= 7){
		if(board_in.grid[to_file][temp_rank].type != VACANT){
			if(board_in.grid[to_file][temp_rank].owner != moving && (board_in.grid[to_file][temp_rank].type == ROOK || board_in.grid[to_file][temp_rank].type == QUEEN || board_in.grid[to_file][temp_rank].type == KING)){
				array_out.array[moves_index++] = move_new(to_file, temp_rank, to_file, to_rank, 0);
			}
			break;
		}
	}
	//From below
	temp_rank = to_rank-1;
	while(temp_rank-- >= 0){
		if(board_in.grid[to_file][temp_rank].type != VACANT){
			if(board_in.grid[to_file][temp_rank].owner != moving && (board_in.grid[to_file][temp_rank].type == ROOK || board_in.grid[to_file][temp_rank].type == QUEEN || board_in.grid[to_file][temp_rank].type == KING)){
				array_out.array[moves_index++] = move_new(to_file, temp_rank, to_file, to_rank, 0);
			}
			break;
		}
	}
	//Check for horizontal attacks
	//From the right
	temp_file = to_file+1;
	while(temp_rank++ <= 7){
		if(board_in.grid[temp_file][to_rank].type != VACANT){
			if(board_in.grid[temp_file][to_rank].owner != moving && (board_in.grid[temp_file][to_rank].type == ROOK || board_in.grid[temp_file][to_rank].type == QUEEN || board_in.grid[temp_file][to_rank].type == KING)){
				array_out.array[moves_index++] = move_new(temp_file, to_rank, to_file, to_rank, 0);
			}
			break;
		}
	}
	//From the left
	temp_file = to_file-1;
	while(temp_rank-- >= 0){
		if(board_in.grid[temp_file][to_rank].type != VACANT){
			if(board_in.grid[temp_file][to_rank].owner != moving && (board_in.grid[temp_file][to_rank].type == ROOK || board_in.grid[temp_file][to_rank].type == QUEEN || board_in.grid[temp_file][to_rank].type == KING)){
				array_out.array[moves_index++] = move_new(temp_file, to_rank, to_file, to_rank, 0);
			}
			break;
		}
	}
	//Check for diagonal attacks
	
	//Check for knight attacks
	
	//Check for en passant attack
	
	//Sets the entry after the last defined entry to an invalid move, to mark the end of the array
	array_out.array[moves_index] = move_invalid();
	return array_out;
}
//******************************************************************
//					Board Analyisis Functions						
//******************************************************************
//Checks if the given position is checkmate (assuming it is already check)
//Undefined behavior if moving player is not in check
//TODO: Implement
int eval_checkmate(struct board boardstate){
	return 0;
}

//Returns material evaluation in centipawns, where:
//pawn = 100, bishop or knight = 300, rook = 500, queen = 900
//White pieces add, black pieces subtract
int eval_material(struct board boardstate){
	int out = 0;
	//Iterates over entire board, adding or subtracting
	//from out, depending on piece type and owner
	for(int file = 0; file <= 7; file++){
		for(int rank = 0; rank <= 7; rank++){
			switch(boardstate.grid[file][rank].type){
				case PAWN:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 1;
					}
					else{
						out = out - 1;
					}
					break;
				case BISHOP:
				case KNIGHT:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 3;
					}
					else{
						out = out - 3;
					}
					break;
				case ROOK:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 5;
					}
					else{
						out = out - 5;
					}
					break;
				case QUEEN:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 9;
					}
					else{
						out = out - 9;
					}
					break;
			}
			//if(boardstate.grid[file][rank].type != VACANT)printf("file: %d, rank: %d, type: %d, owner: %d, eval: %d\n", file, rank, boardstate.grid[file][rank].type, boardstate.grid[file][rank].owner, out);
		}
	}
	return out;
}

//Evaluates advantage from positioning, + values = white advantage, - values = black advantage
//TODO:Develop some sort of positioning valuation algorithm, and implement
//Considerations:check, threats, reinforcement, development, castling, control (esp. center), strength of bishops (blocked by pawns, open/closed game), open rooks, safety of kings
int eval_position(struct board boardstate){
	return 0;
}