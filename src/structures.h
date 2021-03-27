/*
 * structures.h -- Defines some data structures for use in this program:
 * 	piece: Stores properties of a single piece
 * 	move:  Stores properties of a single move
 * 	board: Stores properties of a single boardstate
 * 	grid: Stores 8x8 grid of pieces
 *  Also defines some keywords and types for use elsewhere
*/
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
	unsigned int owner:1;
};
typedef struct piece piece;
//Stores properties of a single move
//Castles must consist of the king moving to it's destination tile
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
	unsigned int promote:3;
};
typedef struct move move;
//Stores properties of a single boardstate
struct board{
	//8x8 array of pieces, where NULL represents an empty tile
	//First array is file, second array is rank
	piece grid[8][8];
	//1 = white's turn, 0 = black's turn
	unsigned int to_move:1;
	//Moves since last "non-reversible" (pawn movement, capture) move
	//Game is automatically drawn at 75 such moves, and can optionally be drawn be either player after 50 such moves*
	unsigned int draw_counter:7;
	//Castling rights
	//1 = that castle is still possible, 0 = not possible
	unsigned int white_kingside:1;
	unsigned int white_queenside:1;
	unsigned int black_kingside:1;
	unsigned int black_queenside:1;
	//En passant handling
	unsigned int en_passant_valid:1;
	unsigned int en_passant_file:3;
	unsigned int en_passant_rank:3;
};
typedef struct board board;

piece new_piece(int type, int owner);
move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote);
board board_new();
board board_move(struct board board_in, struct move move_in);
board _piece_move(struct board board_in, int from_file, int from_rank, int to_file, int to_rank);


