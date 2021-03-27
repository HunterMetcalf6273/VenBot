#include <stdio.h>
#include "structures.h"

//Checks if checkmate (in 1 move) is possible for
//the moving player
//TODO: Implement
int eval_checkmate(struct board boardstate){
	return 0;
}

int eval_material(struct board boardstate){
	//Material evaluation in pawns, where:
	//pawn = 1, bishop or knight = 3, rook = 5, queen = 9
	//White pieces add, black pieces subtract
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