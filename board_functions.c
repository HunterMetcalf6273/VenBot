#include "structures.h"
#include "piece_functions.h"
#include <stdio.h>
#include <string.h>

//Creates a board representing the initial Chess setup
struct board new_game(){
	struct board out;
	out.to_move = WHITE;
	out.draw_counter = 0;
	out.grid[0][1] = new_piece(0, 1, PAWN, WHITE);
	out.grid[1][1] = new_piece(1, 1, PAWN, WHITE);
	out.grid[2][1] = new_piece(2, 1, PAWN, WHITE);
	out.grid[3][1] = new_piece(3, 1, PAWN, WHITE);
	out.grid[4][1] = new_piece(4, 1, PAWN, WHITE);
	out.grid[5][1] = new_piece(5, 1, PAWN, WHITE);
	out.grid[6][1] = new_piece(6, 1, PAWN, WHITE);
	out.grid[7][1] = new_piece(7, 1, PAWN, WHITE);
	out.grid[0][6] = new_piece(0, 6, PAWN, BLACK);
	out.grid[1][6] = new_piece(1, 6, PAWN, BLACK);
	out.grid[2][6] = new_piece(2, 6, PAWN, BLACK);
	out.grid[3][6] = new_piece(3, 6, PAWN, BLACK);
	out.grid[4][6] = new_piece(4, 6, PAWN, BLACK);
	out.grid[5][6] = new_piece(5, 6, PAWN, BLACK);
	out.grid[6][6] = new_piece(6, 6, PAWN, BLACK);
	out.grid[7][6] = new_piece(7, 6, PAWN, BLACK);
	out.grid[0][0] = new_piece(0, 0, ROOK, WHITE);
	out.grid[1][0] = new_piece(1, 0, KNIGHT, WHITE);
	out.grid[2][0] = new_piece(2, 0, BISHOP, WHITE);
	out.grid[3][0] = new_piece(3, 0, QUEEN, WHITE);
	out.grid[4][0] = new_piece(4, 0, KING, WHITE);
	out.grid[5][0] = new_piece(5, 0, BISHOP, WHITE);
	out.grid[6][0] = new_piece(6, 0, KNIGHT, WHITE);
	out.grid[7][0] = new_piece(7, 7, ROOK, BLACK);
	out.grid[1][7] = new_piece(1, 7, KNIGHT, BLACK);
	out.grid[2][7] = new_piece(2, 7, BISHOP, BLACK);
	out.grid[3][7] = new_piece(3, 7, QUEEN, BLACK);
	out.grid[4][7] = new_piece(4, 7, KING, BLACK);
	out.grid[5][7] = new_piece(5, 7, BISHOP, BLACK);
	out.grid[6][7] = new_piece(6, 7, KNIGHT, BLACK);
	out.grid[7][7] = new_piece(7, 7, ROOK, BLACK);
	for(int i = 0; i <= 7; i++){
		for(int j = 2; j <= 5; j++){
			out.grid[i][j] = new_piece(i, j, VACANT, WHITE);
		}
	}
	return out;
}

//Given a position in fen format, returns defined board
//TODO: Implement
struct board board_from_fen(char in[]){
	int file, rank, index;
	struct board out;
	index = 0;
	while(in[index] != '\0'){
		
	}
}

//Given a move-chain and board, makes the given moves on the given board
//and returns the final board
//struct board board_from_moves(struct move cur, struct board boardstate){
//}