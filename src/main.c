#include <stdio.h>
#include "structures.h"
#include "analysis.h"

void main(int argc, char **argv)
{
	board test = board_new();
	move out;
	out.from_file = 3;
	out.from_rank = 0;
	out.to_file = 7;
	out.to_rank = 7;
	out.promote = 0;
	test = board_move(test, out);
	printf("%d", eval_material(test));
}
