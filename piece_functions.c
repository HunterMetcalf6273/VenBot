#include<structures.h>

//Returns a piece, given its location, type, and owner
struct piece new_piece(int file, int rank, int type, int owner){
	struct piece out;
	out.file = file;
	out.rank = rank;
	out.type = type;
	out.owner = owner;
	return out;
}