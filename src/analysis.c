#include <stdio.h>
#include "structures.h"
#include "analysis.h"

//Returns complete eval for given boardstate
//If eval_checkmate != 0, return eval_checkmate
//Else return eval_material + eval_position
int eval_overall(struct board boardstate){
	int checkmate = eval_checkmate(boardstate);
	if(checkmate != 0) return checkmate;
	return eval_material(boardstate) + eval_position(boardstate);
}

//Checks if checkmate (in 1 move) is possible for
//the moving player
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
			if(boardstate.grid[file][rank].type != VACANT)printf("file: %d, rank: %d, type: %d, owner: %d, eval: %d\n", file, rank, boardstate.grid[file][rank].type, boardstate.grid[file][rank].owner, out);
		}
	}
	return out;
}
//Evaluates advantage from positioning, + values = white advantage, - values = black advantage
//TODO:Develop some sort of positioning valuation algorithm, and implement
//Considerations:check, threats, development, castling, control (esp. center), strength of bishops (blocked by pawns, open/closed game), open rooks
int eval_position(struct board boardstate){
	return 0;
}