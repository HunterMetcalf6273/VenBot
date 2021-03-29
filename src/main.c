#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "analysis.h"

void main(int argc, char **argv){
	char input[1024];
	char* pointer;
	board cur_board;
	int max_depth = 8;
	//Unimplemented commands: debug, setoption, register, ucinewgame, stop, ponderhit, info
	while(1){
		fgets(input, 1024, stdin);
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
			if(strncmp(input, "position startpos", 17) == 0) cur_board = board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			else cur_board = board_from_fen(pointer);
			pointer = strstr(pointer, "moves")+6;
			//TODO: Investigate whether or not pointer will always equal 1 on a failed strstr, depending on platform and input
			while(pointer != 1){
				cur_board = board_move(cur_board, move_from_string(pointer));
				pointer = strchr(pointer, ' ') + 1;
			}
			
		}
		//Find best move (ignore other parameters for now)
		else if(strncmp(input, "go", 2) == 0){
			fflush(stdout);
		}
		//Exit program
		else if(strncmp(input, "quit", 4) == 0){
			return;
		}
	}
}
