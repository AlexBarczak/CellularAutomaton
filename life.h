// predefinition of all functions in life.c in case you want to use them in main without 
// the compiler screaming at you
#ifndef LIFE
#define LIFE

int initialize_board(char** board, int width, int height);
void print_board(char** board, int width, int height);
int count_neighbours(char** board, int y, int x, int width, int height);
void life_calculate_next_generation(char** new_board, char** old_board, int width, int height);
void run_life(int width, int height, int length);

#endif