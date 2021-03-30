#include "node_funcs.h"
#include "analysis.h"
#include "board_funcs.h"
#include "move_funcs.h"
#include <stdio.h>

node node_new(board board_in, int depth){
	node out;
	out.sboard = board_in;
	out.depth = depth;
	return out;
}

//Creates a board_node from given parameters (makes move move_in on board board_in; does NOT copy board_in to sboard)
//Only intended to be called by eval_board_node
node node_move(board board_in, move move_in, int depth){
	node out;
	out.sboard = board_move(board_in, move_in);
	out.last_move = move_in;
	out.depth = depth;
	return out;
}

//Finds the best move from the given position, considering children nodes out to max_depth
move node_best_move(node node_in, int max_depth, int alpha, int beta){
	int best, out_index, child_value, children_index;
	node_array children;
	char out_string[6];
	children_index = 0;
	out_index = 0;
	children = board_legal_states(node_in.sboard, node_in.depth+1);
	//If no legal moves, return invalid move
	if(children.array[0].sboard.draw_counter == 127) return move_invalid();
	//White to choose next move
	if(node_in.sboard.to_move == WHITE){
		best = -2147483648;
		while(children.array[children_index].sboard.draw_counter != 127){
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			move_to_string(children.array[children_index].last_move, out_string);
			//fprintf(stdout, "info currmove %s currmovenumber %d", out_string, children_index+1);
			//fflush(stdout);
			if(child_value > best){
				best = child_value;
				out_index = children_index;
			}
			if(best > alpha) alpha = best;
			//if(beta <= alpha) break;
			children_index++;
		}
	}
	//Black to choose next move
	else{
		best = 2147483647;
		while(children.array[children_index].sboard.draw_counter != 127){
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			move_to_string(children.array[children_index].last_move, out_string);
			//fprintf(stdout, "info currmove %s currmovenumber %d", out_string, children_index+1);
			//fflush(stdout);
			if(child_value < best){
				best = child_value;
				out_index = children_index;
			}
			if(best <= beta) beta = best;
			//if(beta <= alpha) break;
			children_index++;
		}
	}
	return children.array[out_index].last_move;
}

int node_eval(node node_in, int max_depth, int alpha, int beta){
	int out, child_value, children_index;
	node_array children;
	children_index = 0;
	children = board_legal_states(node_in.sboard, node_in.depth+1);
	//If no legal moves, return result of game
	if(children.array[0].sboard.draw_counter == 127) return eval_result(node_in.sboard);
	//If we aren't at max_depth, calculate next layer
	if(node_in.depth < max_depth){
		//White to choose next move
		if(node_in.sboard.to_move != BLACK){
			out = -2147483648;
			while(children.array[children_index].sboard.draw_counter != 127){
				child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
				if(child_value > out) out = child_value;
				if(out > alpha) alpha = out;
				if(beta <= alpha) break;
				children_index++;
			}
		}
		//Black to choose next move
		else{
			out = 2147483647;
			while(children.array[children_index].sboard.draw_counter != 127){
				child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
				if(child_value < out) out = child_value;
				if(out <= beta) beta = out;
				if(beta <= alpha) break;
				children_index++;
			}
		}
	}
	//If we are at max depth, calculate value of current boardstate
	else{
		out = eval_position(node_in.sboard);
	}
	return out;
}
