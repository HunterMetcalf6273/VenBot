//node_funcs.c -- Defines the following functions relating to node structs:
	//node node_new(board board_in, int depth)
	//	Returns a new node, given a board and depth
	//node node_move(board board_in, move move_in, int depth)
	//	Returns a new node, with the new node's sboard being the board resulting from move_in being performed on board_in
	//void node_swap(node *pos1, node *pos2)
	//	Swaps the position of two nodes in an array 
	//void node_array_sort(node_array *array, int length)
	//	Sorts a given node array of given length, according to move priority
	//int node_array_length(node_array *array)
	//	Returns the length of a given node array, not considering the terminator node
	//move node_best_move(node node_in, int max_depth, int alpha, int beta)
	//	Returns the calculated best move from node_in.sboard, considering all children out to max_depth
	//int node_eval(node node_in, int max_depth, int alpha, int beta)
	//	Returns the calculate value of node_in.sboard, considering all children out to max_depth

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
node node_move(board board_in, move move_in, int depth){
	node out;
	out.sboard = board_move(board_in, &move_in);
	out.last_move = move_in;
	out.depth = depth;
	return out;
}

//Given two node pointers, swaps their memory values
void node_swap(node *pos1, node *pos2){
	node temp = *pos1;
	*pos1 = *pos2;
	*pos2 = temp;
}

//Given a pointer to a node array, sorts the array
void node_array_sort(node_array *array, int length){
	int index_left, index_right;
	for (index_left = 0; index_left < length-1; index_left++){
		for(index_right = 0; index_right < length - index_left - 1; index_right++){
			if(array->array[index_right].last_move.priority < array->array[index_right+1].last_move.priority) node_swap(&array->array[index_right], &array->array[index_right+1]);
		}
	}
}

//Counts the number of valid values in *array (not counting the terminator node)
int node_array_length(node_array *array){
	int out, index;
	index = 0;
	out = 0;
	while(array->array[index].sboard.draw_counter != 127){
		index++;
		out++;
	}
	return out;
}

//Recursively finds the best move from the given position, considering children nodes out to max_depth
move node_best_move(node node_in, int max_depth, int alpha, int beta){
	int best, out_index, child_value, children_index;
	node_array children;
	children_index = 0;
	out_index = 0;
	children = board_legal_states(node_in.sboard, node_in.depth+1);
	node_array_sort(&children, node_array_length(&children));	
	//If no legal moves, return invalid move, after setting score
	if(children.array[0].sboard.draw_counter == 127){
		score = eval_result(node_in.sboard);
		return move_invalid();
	}
	//White to choose next move
	if(node_in.sboard.to_move == WHITE){
		best = BLACK_CHECKMATE;
		//Iterate over entire children array
		while(children.array[children_index].sboard.draw_counter != 127){
			//Determine value of each child
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			if(child_value > best){
				best = child_value;
				out_index = children_index;
			}
			if(best > alpha) alpha = best;
			children_index++;
		}
	}
	//Black to choose next move
	else{
		best = WHITE_CHECKMATE;
		while(children.array[children_index].sboard.draw_counter != 127){
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			if(child_value < best){
				best = child_value;
				out_index = children_index;
			}
			if(best < beta) beta = best;
			children_index++;
		}
	}
	score = best;
	return children.array[out_index].last_move;
}

//Recursively determines the value of the given node, considering all of its children out to max_depth
int node_eval(node node_in, int max_depth, int alpha, int beta){
	int out, child_value, children_index;
	node_array children;
	//If at max depth, return value of boardstate
	if(node_in.depth >= max_depth) return eval_board(node_in.sboard);
	children_index = 0;
	//Generate children boardstates
	children = board_legal_states(node_in.sboard, node_in.depth+1);
	//Sort possible boardstates according to move priority
	node_array_sort(&children, node_array_length(&children));
	//If no legal moves, return result of game
	if(children.array[0].sboard.draw_counter == 127) return eval_result(node_in.sboard);
	//White to choose next move
	if(node_in.sboard.to_move != BLACK){
		out = BLACK_CHECKMATE;
		//Iterate over entire children array
		while(children.array[children_index].sboard.draw_counter != 127){
			//Recursive call for each child
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			if(child_value > WHITE_CHECKMATE-10000){
				//Prefer shorter checkmates
				child_value -= node_in.depth;
			}
			if(child_value > out) out = child_value;
			if(out > alpha) alpha = out;
			if(beta <= alpha) break;
			children_index++;
		}
	}
	//Black to choose next move
	else{
		out = WHITE_CHECKMATE;
		while(children.array[children_index].sboard.draw_counter != 127){
			child_value = node_eval(children.array[children_index], max_depth, alpha, beta);
			if(child_value < BLACK_CHECKMATE+10000){
				child_value += node_in.depth;
			}
			if(child_value < out) out = child_value;
			if(out < beta) beta = out;
			if(beta <= alpha) break;
			children_index++;
		}
	}
	return out;
}
