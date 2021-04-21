//piece_funcs.c -- Defines the following commands related to piece structs:
	//piece piece_new(int type, int owner)
	//	Returns a new string, given a type and owner
	//piece piece_invalid()
	//	Returns an invalid piece, used to mark the end of piece arrays

#include "piece_funcs.h"

//Returns a piece, given its location, type, and owner
piece piece_new(int type, int owner){
	piece out;
	out.type = type;
	out.owner = owner;
	return out;
}

//Returns an invalid piece (type == 7)
piece piece_invalid(){
	piece out;
	out.type = 7;
	return out;
}
