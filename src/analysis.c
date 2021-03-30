#include <stdio.h>
#include "structures.h"
#include "analysis.h"

//******************************************************************
//					Helper Functions						
//******************************************************************

//Checks if the moving player is in check
bool eval_check(board board_in){
	int king_location_file, king_location_rank;
	piece_array threats;
	king_location_file = 0;
	king_location_rank = 0;
	//Iterate over grid until moving player's king is found
	while(king_location_file < 8){
		while(king_location_rank < 8){
			if(board_in.grid[king_location_file][king_location_rank].type == KING && board_in.grid[king_location_file][king_location_rank].owner == board_in.to_move) goto end;
			king_location_rank++;
		}
		king_location_rank = 0;
		king_location_file++;
	}
	end:
	//Check if the opposing player could capture the king, were it their turn
	threats = eval_array_captures(board_in, king_location_file, king_location_rank, !board_in.to_move);
	//If the first entry doesn't contain a valid piece, then there are no valid threats to the given tile, so the king is not in check
	//TODO: implement with king-specific function instead, to avoid unnecessarily calculating entire piece_array
	return threats.array[0].type != 7;
}
//Returns an array of all pieces the given player owns which could capture the given tile
//Useful either to determine how threatened a tile is, or to determine how reinforced a tile is
//TODO: Test
piece_array eval_array_captures(board board_in, int to_file, int to_rank, bool moving){
	int temp_file, temp_rank, pieces_index;
	piece_array array_out;
	pieces_index = 0;
	//Check for vertical attacks
	//From above
	temp_rank = to_rank+1;
	while(temp_rank <= 7){
		if(board_in.grid[to_file][temp_rank].type != VACANT){
			if(board_in.grid[to_file][temp_rank].owner == moving && 
				(board_in.grid[to_file][temp_rank].type == ROOK || 
				board_in.grid[to_file][temp_rank].type == QUEEN || 
				(board_in.grid[to_file][temp_rank].type == KING && to_rank + 1 == temp_rank)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[to_file][temp_rank].type, moving);
			}
			break;
		}
		temp_rank++;
	}
	//From below
	temp_rank = to_rank-1;
	while(temp_rank >= 0){
		if(board_in.grid[to_file][temp_rank].type != VACANT){
			if(board_in.grid[to_file][temp_rank].owner == moving && 
				(board_in.grid[to_file][temp_rank].type == ROOK || 
				board_in.grid[to_file][temp_rank].type == QUEEN || 
				(board_in.grid[to_file][temp_rank].type == KING && to_rank - 1 == temp_rank)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[to_file][temp_rank].type, moving);
			}
			break;
		}
		temp_rank--;
	}
	//Check for horizontal attacks
	//From the right
	temp_file = to_file+1;
	while(temp_file <= 7){
		if(board_in.grid[temp_file][to_rank].type != VACANT){
			if(board_in.grid[temp_file][to_rank].owner == moving && 
				(board_in.grid[temp_file][to_rank].type == ROOK || 
				board_in.grid[temp_file][to_rank].type == QUEEN || 
				(board_in.grid[temp_file][to_rank].type == KING && to_file + 1 == temp_file)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][to_rank].type, moving);
			}
			break;
		}
		temp_file++;
	}
	//From the left
	temp_file = to_file-1;
	while(temp_file >= 0){
		if(board_in.grid[temp_file][to_rank].type != VACANT){
			if(board_in.grid[temp_file][to_rank].owner == moving && 
				(board_in.grid[temp_file][to_rank].type == ROOK || 
				board_in.grid[temp_file][to_rank].type == QUEEN || 
				(board_in.grid[temp_file][to_rank].type == KING && to_file - 1 == temp_file)))
					{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][to_rank].type, moving);
			}
			break;
		}
		temp_file--;
	}
	//Check for diagonal attacks
	//Up-Right
	temp_file = to_file+1;
	temp_rank = to_rank+1;
	while(temp_file <= 7 && temp_rank <=7){
		if(board_in.grid[temp_file][temp_rank].type != VACANT){
			if(board_in.grid[temp_file][temp_rank].owner == moving && (
				board_in.grid[temp_file][temp_rank].type == BISHOP || 
				board_in.grid[temp_file][temp_rank].type == QUEEN || 
				((board_in.grid[temp_file][temp_rank].type == KING || (!moving && board_in.grid[temp_file][temp_rank].type == PAWN))&& to_file + 1 == temp_file)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][temp_rank].type, moving);
			}
			break;
		}
		temp_file++;
		temp_rank++;
	}
	//Down-Right
	temp_file = to_file+1;
	temp_rank = to_rank-1;
	while(temp_file <= 7 && temp_rank >=0){
		if(board_in.grid[temp_file][temp_rank].type != VACANT){
			if(board_in.grid[temp_file][temp_rank].owner == moving && (
				board_in.grid[temp_file][temp_rank].type == BISHOP || 
				board_in.grid[temp_file][temp_rank].type == QUEEN || 
				((board_in.grid[temp_file][temp_rank].type == KING || (moving && board_in.grid[temp_file][temp_rank].type == PAWN))&& to_file + 1 == temp_file)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][temp_rank].type, moving);
			}
			break;
		}
		temp_file++;
		temp_rank--;
	}
	//Up-Left
	temp_file = to_file-1;
	temp_rank = to_rank+1;
	while(temp_file >= 0 && temp_rank <=7){
		if(board_in.grid[temp_file][temp_rank].type != VACANT){
			if(board_in.grid[temp_file][temp_rank].owner == moving && (
				board_in.grid[temp_file][temp_rank].type == BISHOP || 
				board_in.grid[temp_file][temp_rank].type == QUEEN || 
				((board_in.grid[temp_file][temp_rank].type == KING ||
				(!moving && board_in.grid[temp_file][temp_rank].type == PAWN))
				&& to_file - 1 == temp_file)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][temp_rank].type, moving);
			}
			break;
		}
		temp_file--;
		temp_rank++;
	}
	//Down-Left
	temp_file = to_file-1;
	temp_rank = to_rank-1;
	while(temp_file >= 0 && temp_rank >=0){
		if(board_in.grid[temp_file][temp_rank].type != VACANT){
			if(board_in.grid[temp_file][temp_rank].owner == moving && (
				board_in.grid[temp_file][temp_rank].type == BISHOP || 
				board_in.grid[temp_file][temp_rank].type == QUEEN || 
				((board_in.grid[temp_file][temp_rank].type == KING || (moving && board_in.grid[temp_file][temp_rank].type == PAWN))&& to_file - 1 == temp_file)))
				{
				array_out.array[pieces_index++] = new_piece(board_in.grid[temp_file][temp_rank].type, moving);
			}
			break;
		}
		temp_file--;
		temp_rank--;
	}
	//Check for knight attacks
	//Counting clockwise:
	//1
	if(to_file+1 <= 7 && to_rank+2 <= 7 && board_in.grid[to_file+1][to_rank+2].type == KNIGHT && board_in.grid[to_file+1][to_rank+2].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//2
	if(to_file+2 <= 7 && to_rank+1 <= 7 && board_in.grid[to_file+2][to_rank+1].type == KNIGHT && board_in.grid[to_file+2][to_rank+1].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//3
	if(to_file+2 <= 7 && to_rank-1 >= 0 && board_in.grid[to_file+2][to_rank-1].type == KNIGHT && board_in.grid[to_file+2][to_rank-1].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//4
	if(to_file+1 <= 7 && to_rank-2 >= 0 && board_in.grid[to_file+1][to_rank-2].type == KNIGHT && board_in.grid[to_file+1][to_rank-2].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//5
	if(to_file-1 >= 0 && to_rank-2 >= 0 && board_in.grid[to_file-1][to_rank-2].type == KNIGHT && board_in.grid[to_file-1][to_rank-2].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//6
	if(to_file-2 >= 0 && to_rank-1 >= 0 && board_in.grid[to_file-2][to_rank-1].type == KNIGHT && board_in.grid[to_file-2][to_rank-1].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//7
	if(to_file-2 >= 0 && to_rank+1 <= 7 && board_in.grid[to_file-2][to_rank+1].type == KNIGHT && board_in.grid[to_file-2][to_rank+1].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//8
	if(to_file-1 >= 0 && to_rank+2 <= 7 && board_in.grid[to_file-1][to_rank+2].type == KNIGHT && board_in.grid[to_file-1][to_rank+2].owner == moving) array_out.array[pieces_index++] = new_piece(KNIGHT, moving);
	//Check for en passant attack
	if(board_in.en_passant_valid && to_file == board_in.en_passant_file && to_rank == board_in.en_passant_rank){
		if(to_file >= 1 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file-1][to_rank].owner == moving && board_in.grid[to_file-1][to_rank].type == PAWN) array_out.array[pieces_index++] = new_piece(PAWN, moving);
		if(to_file <= 6 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file+1][to_rank].owner == moving && board_in.grid[to_file+1][to_rank].type == PAWN) array_out.array[pieces_index++] = new_piece(PAWN, moving);
	}
	//Sets the entry after the last defined entry to an invalid move, to mark the end of the array
	array_out.array[pieces_index] = piece_invalid();
	return array_out;
}
//******************************************************************
//					Board Analyisis Functions						
//******************************************************************
//Given a position with no legal moves, determines the winner
//0 = black, 1 = white, -1 = draw
//Undefined behavior if given a position with legal moves
//TODO: Implement
int eval_result(struct board boardstate){
	if(eval_check(boardstate)) return !boardstate.to_move;
	return -1;
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