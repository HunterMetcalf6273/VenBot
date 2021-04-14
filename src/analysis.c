#include <stdio.h>
#include "piece_funcs.h"
#include "move_funcs.h"
#include "board_funcs.h"
#include "node_funcs.h"
#include "analysis.h"

//******************************************************************
//					Helper Functions						
//******************************************************************

//Checks if the moving player is in check
bool eval_check(board board_in){
	int king_location_file, king_location_rank;
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
	return _eval_array_captures(board_in, king_location_file, king_location_rank, !board_in.to_move);
}

//Returns an array of all pieces the given player owns which could capture the given tile
//Useful either to determine how threatened a tile is, or to determine how reinforced a tile is
piece* eval_array_captures(board board_in, int to_file, int to_rank, bool moving){
	int temp_file, temp_rank, pieces_index;
	static piece array_out[64];
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
				array_out[pieces_index++] = piece_new(board_in.grid[to_file][temp_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[to_file][temp_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][to_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][to_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][temp_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][temp_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][temp_rank].type, moving);
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
				array_out[pieces_index++] = piece_new(board_in.grid[temp_file][temp_rank].type, moving);
			}
			break;
		}
		temp_file--;
		temp_rank--;
	}
	//Check for knight attacks
	//Counting clockwise:
	//1
	if(to_file+1 <= 7 && to_rank+2 <= 7 && board_in.grid[to_file+1][to_rank+2].type == KNIGHT && board_in.grid[to_file+1][to_rank+2].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//2
	if(to_file+2 <= 7 && to_rank+1 <= 7 && board_in.grid[to_file+2][to_rank+1].type == KNIGHT && board_in.grid[to_file+2][to_rank+1].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//3
	if(to_file+2 <= 7 && to_rank-1 >= 0 && board_in.grid[to_file+2][to_rank-1].type == KNIGHT && board_in.grid[to_file+2][to_rank-1].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//4
	if(to_file+1 <= 7 && to_rank-2 >= 0 && board_in.grid[to_file+1][to_rank-2].type == KNIGHT && board_in.grid[to_file+1][to_rank-2].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//5
	if(to_file-1 >= 0 && to_rank-2 >= 0 && board_in.grid[to_file-1][to_rank-2].type == KNIGHT && board_in.grid[to_file-1][to_rank-2].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//6
	if(to_file-2 >= 0 && to_rank-1 >= 0 && board_in.grid[to_file-2][to_rank-1].type == KNIGHT && board_in.grid[to_file-2][to_rank-1].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//7
	if(to_file-2 >= 0 && to_rank+1 <= 7 && board_in.grid[to_file-2][to_rank+1].type == KNIGHT && board_in.grid[to_file-2][to_rank+1].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//8
	if(to_file-1 >= 0 && to_rank+2 <= 7 && board_in.grid[to_file-1][to_rank+2].type == KNIGHT && board_in.grid[to_file-1][to_rank+2].owner == moving) array_out[pieces_index++] = piece_new(KNIGHT, moving);
	//Check for en passant attack
	if(board_in.en_passant_valid && to_file == board_in.en_passant_file && to_rank == board_in.en_passant_rank){
		if(to_file >= 1 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file-1][to_rank].owner == moving && board_in.grid[to_file-1][to_rank].type == PAWN) array_out[pieces_index++] = piece_new(PAWN, moving);
		if(to_file <= 6 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file+1][to_rank].owner == moving && board_in.grid[to_file+1][to_rank].type == PAWN) array_out[pieces_index++] = piece_new(PAWN, moving);
	}
	//Sets the entry after the last defined entry to an invalid move, to mark the end of the array
	array_out[pieces_index] = piece_invalid();
	return array_out;
}

//Special version of eval_array_captures which only returns whether or not any capture exists, not from what kind of piece
//Useful for check detection, primarily
bool _eval_array_captures(board board_in, int to_file, int to_rank, bool moving){
	int temp_file, temp_rank;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
			}
			break;
		}
		temp_file--;
		temp_rank--;
	}
	//Check for knight attacks
	//Counting clockwise:
	//1
	if(to_file+1 <= 7 && to_rank+2 <= 7 && board_in.grid[to_file+1][to_rank+2].type == KNIGHT && board_in.grid[to_file+1][to_rank+2].owner == moving) return true;
	//2
	if(to_file+2 <= 7 && to_rank+1 <= 7 && board_in.grid[to_file+2][to_rank+1].type == KNIGHT && board_in.grid[to_file+2][to_rank+1].owner == moving) return true;
	//3
	if(to_file+2 <= 7 && to_rank-1 >= 0 && board_in.grid[to_file+2][to_rank-1].type == KNIGHT && board_in.grid[to_file+2][to_rank-1].owner == moving) return true;
	//4
	if(to_file+1 <= 7 && to_rank-2 >= 0 && board_in.grid[to_file+1][to_rank-2].type == KNIGHT && board_in.grid[to_file+1][to_rank-2].owner == moving) return true;
	//5
	if(to_file-1 >= 0 && to_rank-2 >= 0 && board_in.grid[to_file-1][to_rank-2].type == KNIGHT && board_in.grid[to_file-1][to_rank-2].owner == moving) return true;
	//6
	if(to_file-2 >= 0 && to_rank-1 >= 0 && board_in.grid[to_file-2][to_rank-1].type == KNIGHT && board_in.grid[to_file-2][to_rank-1].owner == moving) return true;
	//7
	if(to_file-2 >= 0 && to_rank+1 <= 7 && board_in.grid[to_file-2][to_rank+1].type == KNIGHT && board_in.grid[to_file-2][to_rank+1].owner == moving) return true;
	//8
	if(to_file-1 >= 0 && to_rank+2 <= 7 && board_in.grid[to_file-1][to_rank+2].type == KNIGHT && board_in.grid[to_file-1][to_rank+2].owner == moving) return true;
	//Check for en passant attack
	if(board_in.en_passant_valid && to_file == board_in.en_passant_file && to_rank == board_in.en_passant_rank){
		if(to_file >= 1 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file-1][to_rank].owner == moving && board_in.grid[to_file-1][to_rank].type == PAWN) return true;
		if(to_file <= 6 && board_in.grid[to_file][to_rank].owner == !moving && board_in.grid[to_file+1][to_rank].owner == moving && board_in.grid[to_file+1][to_rank].type == PAWN) return true;
	}
	return false;
}
//******************************************************************
//					Board Analyisis Functions						
//******************************************************************
//Given a position with no legal moves, determines the winner
//BLACK_CHECKMATE = black, WHITE_CHECKMATE = white, 0 = draw
//Undefined behavior if given a position with legal moves
int eval_result(struct board boardstate){
	if(eval_check(boardstate)){
		if(boardstate.to_move){
			return BLACK_CHECKMATE;
		}
		else{
			return WHITE_CHECKMATE;
		}
	}
	return 0;
}

//Given a board, calculates the advantage, where positive values represent white advantage and negative values represent black advantage
int eval_board(struct board boardstate){
	int out, white_pawns, black_pawns; //white/black tiles, not owner
	bool white_white_bishop, white_black_bishop, black_white_bishop, black_black_bishop; //owner_tile
	out = 0;
	white_pawns = 0;
	black_pawns = 0;
	white_white_bishop = false;
	white_black_bishop = false;
	black_white_bishop = false;
	black_black_bishop = false;
	//Iterates over entire board, adding or subtracting
	//from out, depending on piece type and owner
	for(int file = 0; file <= 7; file++){
		for(int rank = 0; rank <= 7; rank++){
			switch(boardstate.grid[file][rank].type){
				case PAWN:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 100;
						//Pawns should go towards the enemy
						out += rank*10;
					}
					else{
						out = out - 100;
						out -= 70 - rank*10;
					}
					if(file%2 == 0 && rank%2 == 1) white_pawns++;
					else black_pawns++;
					break;
				case BISHOP:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 350;
						if(file%2 == 0 && rank%2 == 1) white_white_bishop = true;
						else white_black_bishop = true;
						if(rank == 0) out -= 50;
					}
					else{
						out = out - 350;
						if(file%2 == 0 && rank%2 == 1) black_white_bishop = true;
						else black_black_bishop = true;
						if(rank == 7) out += 50;
					}
					break;
				case KNIGHT:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 300;
						//Knights belong in the middle of the board
						if(file >= 2 && file <= 5 && rank >=2 && rank <= 5) out += 25;
						if(file >= 3 && file <= 4 && rank >=3 && rank <= 4) out += 25;
						//Undeveloped pieces are worse
						else if(rank == 0) out -= 50;
					}
					else{
						out = out - 300;
						if(file >= 2 && file <= 5 && rank >=2 && rank <= 5) out -= 25;
						if(file >= 3 && file <= 4 && rank >=3 && rank <= 4) out -= 25;
						else if(rank == 7) out += 50;
					}
					break;
				case ROOK:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 500;
						//Controlling the center files
						if(file == 3 || file == 4) out += 50;
					}
					else{
						out = out - 500;
						if(file == 3 || file == 4) out -= 50;
					}
					break;
				case QUEEN:
					if(boardstate.grid[file][rank].owner == 1){
						out = out + 900;
					}
					else{
						out = out - 900;
					}
					break;
			}
		}
	}
	//Bishops are better with less pawns on their tiles
	if(white_white_bishop) out -= white_pawns * 10;
	if(white_black_bishop) out -= black_pawns * 10;
	if(black_white_bishop) out += white_pawns * 10;
	if(black_black_bishop) out += black_pawns * 10;
	//Check is bad for the moving player
	if(boardstate.to_move && eval_check(boardstate)) out -= 50;
	else if(!boardstate.to_move && eval_check(boardstate)) out +=50;
	//Castling is good
	if(boardstate.grid[1][0].type == KING || boardstate.grid[6][0].type == KING) out+=100;
	if(boardstate.grid[1][7].type == KING || boardstate.grid[6][7].type == KING) out-=100;
	return out;
}