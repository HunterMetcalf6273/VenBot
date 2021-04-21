//move_funcs.c -- Defines the following functions relating to move structs:
	//move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote)
	//	Returns a new move struct from the given parameters
	//move move_invalid()
	//	Returns an invalid move, used to terminate move arrays
	//move move_from_string(char *str)
	//	Returns a move struct from the given long-algebraic string
	//void move_to_string(move move_in, char* out)
	//	Writes a long-algebraic string from the given move struct to the given string pointer
	
#include "move_funcs.h"

//Returns a move, as defined by parameters
move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote){
	move out;
	out.from_file = from_file;
	out.from_rank = from_rank;
	out.to_file = to_file;
	out.to_rank = to_rank;
	out.promote = promote;
	out.priority = 0;
	return out;
}
//Returns a move with the promote field set to 7, to indicate an undefined move
move move_invalid(){
	move out;
	out.promote = 7;
	return out;
}
//Converts a string which starts with a long algebraic chess move (which does not contain an x to mark captures),
//delimited by either a space or a null character, into a move struct
//Undefined behavior if given anything else
move move_from_string(char *str){
	move out;
	out.from_file = *(str) - 97;
	out.from_rank = *(str+1) - 97;
	out.to_file = *(str+2) - 97;
	out.to_rank = *(str+3) - 97;
	switch(*(str+4)){
		case 'q':
			out.promote = QUEEN;
			break;
		case 'r':
			out.promote = ROOK;
			break;
		case 'b':
			out.promote = BISHOP;
			break;
		case 'n':
			out.promote = KNIGHT;
			break;
		default:
			out.promote = 0;
			break;
	}
	return out;
}

//Converts a move into a long algebraic chess move, without an x to mark captures, stored at the given address
void move_to_string(move move_in, char* out){
	out[0] = move_in.from_file + 97;
	out[1] = move_in.from_rank + 49;
	out[2] = ' ';
	out[3] = move_in.to_file + 97;
	out[4] = move_in.to_rank + 49;
	out[5] = '\0';
}