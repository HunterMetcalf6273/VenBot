#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "board_funcs.h"
#include "piece_funcs.h"
#include "move_funcs.h"
#include "node_funcs.h"
#include "analysis.h"

int score;

int main(int argc, char **argv){
	char input[1024];
	char* pointer;
	char out_string[6];
	board cur_board;
	int max_depth = 5;
	//Unimplemented commands: debug, setoption, register, ucinewgame, stop, ponderhit, info
	while(1){
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
			if(strncmp(input, "position startpos", 17) == 0) cur_board = board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			else cur_board = board_from_fen(pointer+4);
			pointer = strstr(pointer, "moves")+6;
			while(pointer -6 != NULL && pointer - 1 != NULL){
				cur_board = board_move(cur_board, move_from_string(pointer));
				pointer = strchr(pointer, ' ') + 1;
			}
			
		}
		//Find best move (ignore other parameters for now)
		else if(strncmp(input, "go", 2) == 0){
			move_to_string(node_best_move(node_new(cur_board, 0), max_depth, BLACK_CHECKMATE, WHITE_CHECKMATE), out_string);
			fprintf(stdout, "info score %d\n", score);
			fprintf(stdout, "bestmove %s\n", out_string);
			fflush(stdout);
		}
		//Exit program
		else if(strncmp(input, "quit", 4) == 0){
			return 0;
		}
		//Testing:
		else if(strncmp(input, "check", 4) == 0){
			if(eval_check(cur_board)) fprintf(stdout, "%d is in check\n", cur_board.to_move);
			else fprintf(stdout, "%d is not in check\n", cur_board.to_move);
			fflush(stdout);
		}
		/*else if(strncmp(input, "boards", 6) == 0){
				moves = board_legal_states(cur_board);
				int out = 0;
				while(moves.array[out].draw_counter != 127){
					out++;
				}
				fprintf(stdout, "done, calculated %d boardstates\n", out);
				if(out == 0){
					fprintf(stdout, "end state, result: %d\n", eval_result(cur_board));
				}
				fflush(stdout);
		}*/
	}
}
