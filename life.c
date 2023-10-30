#include <stdio.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 40

typedef struct board
{
    char ** board;   
} Board;


int initialize_board(Board* board){

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
            board->board[i][j] = ',';
        }
    }

    return 0;
}

void print_board(Board* board){
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%c", board->board[i][j]);
            printf("%c", board->board[i][j]);
        }
        printf("\n");
    }
}

void calculate_next_generation(){
    // TO BE DONE
}

int main(void){

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

    if (initialize_board(current_board) == 1)
    {
        return 1;
    }
    if (initialize_board(previous_board) == 1)
    {
        return 1;
    }    

    current_board->board[HEIGHT/2][WIDTH/2] = '#';

    print_board(current_board);

}