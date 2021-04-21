//main.c -- Provides overall control of the program; accepts the following commands from stdin:
	//uci:
	//	Print uci-compliance information to stdout
	//isready:
	//	Print "readyok" to stdout (to show that the program is ready for other commands)
	//position:
	//	Syntax: position <startpos|FEN String> <moves <moves in long-algebraic notation, delinated by spaces>>
	//	Sets the internal boardstate to the given position, taking either startpos or a valid FEN string as the first parameter,
	//	And then making the moves defined by moves parameter, if present
	//go:
	//	Prints the calculated value of the current boardstate, and the best move in the current boardstate
	//quit:
	//	Exits the program

#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "board_funcs.h"
#include "piece_funcs.h"
#include "move_funcs.h"
#include "node_funcs.h"
#include "analysis.h"

//Used to allow main to print out the calculated value of a position without returning it
int score;

int main(int argc, char **argv){
	char input[1024];
	char out_string[6];
	char* pointer;
	move temp_move;
	board cur_board;
	//Currently hardcoded
	int max_depth = 5;
	//Unimplemented commands: debug, setoption, register, ucinewgame, stop, ponderhit
	while(true){
		//Read in input, either from console or calling program
		fgets(input, 1024, stdin);
		fflush(stdin);
		input[strcspn(input, "\n")] = 0;
		if(strncmp(input, "uci", 3) == 0){
			fprintf(stdout, "id name VenBot\nid author Hunter Metcalf\nuciok\n");
			fflush(stdout);
		}
		else if(strncmp(input, "isready", 7) == 0){
			printf("readyok\n");
			fflush(stdout);
		}
		else if(strncmp(input, "position", 8) == 0){
			pointer = strchr(input, ' ') + 1;
			//If given position startpos, set internal boardstate to starting position
			if(strncmp(input, "position startpos", 17) == 0) cur_board = board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			//Else set boardstate according to the given FEN string
			//Does not check for validity of FEN
			else cur_board = board_from_fen(pointer+4);
			pointer = strstr(pointer, "moves")+6;
			//If the "moves" section is included, make the given moves on the internal boardstate
			//Moves are not checked for validity
			while(pointer -6 != NULL && pointer - 1 != NULL){
				temp_move = move_from_string(pointer);
				cur_board = board_move(cur_board, &temp_move);
				pointer = strchr(pointer, ' ') + 1;
			}
		}
		//Find best move (ignore other parameters for now)
		else if(strncmp(input, "go", 2) == 0){
			//Find the best move from the current boardstate, and convert it into a string that is written into out_string
			move_to_string(node_best_move(node_new(cur_board, 0), max_depth, BLACK_CHECKMATE, WHITE_CHECKMATE), out_string);
			//Print out calculated score and best move
			fprintf(stdout, "info score %d\n", score);
			fprintf(stdout, "bestmove %s\n", out_string);
			fflush(stdout);
		}
		//Exit program
		else if(strncmp(input, "quit", 4) == 0){
			return 0;
		}
	}
}
