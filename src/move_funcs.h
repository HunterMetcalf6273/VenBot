#include "structures.h"
move move_new(int from_file, int from_rank, int to_file, int to_rank, int promote);
move move_invalid();
move move_from_string(char *str);
void move_to_string(move move_in, char* out);