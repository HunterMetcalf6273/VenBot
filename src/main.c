#include <stdio.h>
#include "structures.h"
#include "analysis.h"

void main(int argc, char **argv)
{
	char test_fen[] = "rnbqkbnr/pppppppp/3QQ3/1rQqqQr1/1rQQQQr1/1rrrrrr1/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	board test = board_from_fen(test_fen);
	printf("%d", eval_material(test));
}
