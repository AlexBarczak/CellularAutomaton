#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 40

typedef struct board
{
    char ** board;   
} Board;


int initialize_board(Board* board, int isRand){

    board->board = NULL;
    board->board = (char**)malloc(sizeof(char*)*HEIGHT);

    if (board->board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return 1;
    }

    for (int i = 0; i < HEIGHT; i++){
        board->board[i] = NULL;
        board->board[i] = (char*)malloc(sizeof(char) * WIDTH);
        if (board->board[i] == NULL)
        {
            printf("\nmemory allocation issue\naborting...\n");
            return 1;
        }
        for (int j = 0; j < WIDTH; j++)
        {
            if (isRand)
            {
                board->board[i][j] = rand()%2;
                continue;
            }
            
            board->board[i][j] = 0;
        }
    }

    return 0;
}

void print_board(Board* board){
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (board->board[i][j] == 0)
            {
                printf(" ");
                continue;
            } 
            printf("#");
        }
        printf("\n");
    }
    return;
}

int count_neighbours(Board* board, int y, int x){
    int count = 0;
    for (int yOff = -1; yOff < 2; yOff++){
        for (int xOff = -1; xOff < 2; xOff++){
            if (xOff == yOff && xOff == 0){
                continue;
            }
            
            count += board->board[(y+yOff)%HEIGHT][(x+xOff)%WIDTH];
        }
    }
    return count;
}

void calculate_next_generation(Board* new_board, Board* old_board){
    for(int i = HEIGHT; i < 2*HEIGHT; i++){
        for(int j = WIDTH; j < 2*WIDTH; j++){
            int neighbours = count_neighbours(old_board, i, j);
            
            // if alive cell
            if (old_board->board[i%HEIGHT][j%WIDTH] == 1)
            {
                if (neighbours < 2 || neighbours > 3){
                    new_board->board[i%HEIGHT][j%WIDTH] = 0;
                    continue;
                }
                new_board->board[i%HEIGHT][j%WIDTH] = 1;
                continue;
            }
            
            // if dead cell
            if (neighbours == 3){
                new_board->board[i%HEIGHT][j%WIDTH] = 1;
                continue;
            }
            new_board->board[i%HEIGHT][j%WIDTH] = 0;                    
        }
    }
}


int main(void){
    srand(time(NULL));

    // game of life consists of a 2d array of positions filled with a number of cells in alive or dead 'states'
    // these will be denoted with a value of 0 or 1
    Board* current_board;
    Board* previous_board;

    current_board = (Board*)malloc(sizeof(Board));

    if (current_board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return 1;
    }
    previous_board = (Board*)malloc(sizeof(Board));

    if (previous_board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return 1;
    }

    if (initialize_board(current_board, 1) == 1)
    {
        printf("bad");
        return 1;
    }
    if (initialize_board(previous_board, 1) == 1)
    {
        printf("bad");
        return 1;
    }    

    // making a glider to test for good behaviour in the simulation
    current_board->board[2+15][0+15] = 1;
    current_board->board[2+15][1+15] = 1;
    current_board->board[2+15][2+15] = 1;
    current_board->board[1+15][2+15] = 1;
    current_board->board[0+15][1+15] = 1;

    struct timespec remaining, request = { 0, 50000000 };

    system("clear");
    print_board(current_board);
    nanosleep(&request, &remaining);
    while(1){
        system("clear");
        Board *temp = previous_board;
        previous_board = current_board;
        current_board = temp;
        calculate_next_generation(current_board, previous_board);
        print_board(current_board);
        nanosleep(&request, &remaining);
    }

}