//structures.h -- Defines (and typedefs) some data structures for use in this program:
//	piece: Stores properties of a single piece
//	move:  Stores properties of a single move
//	board: Stores properties of a single boardstate
//	grid: Stores 8x8 grid of pieces
//	move_node: stores a move, its value, its board, and 2 pointers, for use in the possible move tree
//Also defines some keywords for use elsewhere

#include<stdbool.h>

#define VACANT 0
#define PAWN 1
#define KING 2
#define QUEEN 3
#define ROOK 4
#define BISHOP 5
#define KNIGHT 6
#define WHITE 1
#define BLACK 0

//Stores properties of a single piece
struct piece{
	//Type of piece, encoded as:
	//0 = vacant, 1 = pawn, 2 = king, 3 = queen, 4 = rook, 5 = bishop, 6 = knight
	unsigned int type:3;
	//1 = white's piece, 0 = black's piece
	//Never used if type = VACANT; value can be ignored in that case
	bool owner;
};
typedef struct piece piece;
//Stores properties of a single move
//Castles must consist of the king moving to it's destination tile
//Stores properties of a single boardstate
struct board{
	//8x8 array of pieces, where NULL represents an empty tile
	//First array is file, second array is rank
	piece grid[8][8];
	//Moves since last "non-reversible" (pawn movement, capture) move
	//Game is automatically drawn at 75 such moves, and can optionally be drawn be either player after 50 such moves*
	unsigned int draw_counter:7;
	unsigned int en_passant_file:3;
	unsigned int en_passant_rank:3;
	//1 = white's turn, 0 = black's turn
	bool to_move;
	//Castling rights
	//1 = that castle is still possible, 0 = not possible
	bool white_kingside;
	bool white_queenside;
	bool black_kingside;
	bool black_queenside;
	//En passant handling
	bool en_passant_valid;
};
typedef struct board board;
struct move{
	//File and rank of moving piece, encoded as:
	//a thru h = 0 thru 7 for file, 1 thru 8 = 0 thru 7 for rank*/
	unsigned int from_file:3;
	unsigned int from_rank:3;
	//File and rank of destination, encoded as:
	//a thru h = 0 thru 7 for file, 1 thru 8 = 0 thru 7 for rank*/
	unsigned int to_file:3;
	unsigned int to_rank:3;
	//If move is a promotion, stores what to promote to
	//TODO: Consider finding a way to avoid using these 3 bits for every move
	unsigned int promote:3;
};
typedef struct move move;
struct move_node{
	//Stores the board this move results in
	board next_board;
	//Stores the made move
	move current_move;
	//Stores the next move_node downwards (that is, the first calculated move_node from this boardstate)
	struct move_node* down;
	//Store the next move_node leftwards (that is, the next in the list of possible moves from the previous boardstate)
	struct move_node* left;
	//Stores the depth of this node
	unsigned int depth:6;
};
typedef struct move_node move_node;

struct move_list_node{
	move stored_move;
	struct move_list_node* next;
};
typedef struct move_list_node move_list_node;

//Function declarations
board board_new();
board board_from_fen(char in[]);
board _piece_move(struct board board_in, int from_file, int from_rank, int to_file, int to_rank);
board board_move(struct board board_in, struct move move_in);
move_list_node moveboard_get_legal_moves(board in, int from_file, int from_rank);
move_node* board_get_legal_move_list(board in, int depth);
piece new_piece(int type, int owner);
move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote);
move_node move_node_new(int depth);
int move_node_eval(move_node* node);

