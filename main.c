#include <stdio.h>
#include "board_functions.h"
#include "analysis.h"
#include "structures.h"

void main(int argc, char **argv)
{
	struct board theboard = new_game();
	int out = eval_material(theboard);
	printf("%d\n", out);
	printf("Yeet");
}
