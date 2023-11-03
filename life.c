#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "life.h"

int initialize_board(char** board, int width, int height){

    if (board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return 1;
    }

    for (int i = 0; i < height; i++){
        board[i] = NULL;
        board[i] = (char*)malloc(sizeof(char) * width);
        if (board[i] == NULL)
        {
            printf("\nmemory allocation issue\naborting...\n");
            return 1;
        }
        for (int j = 0; j < width; j++)
        {
            board[i][j] = rand()%2;
        }
    }

    return 0;
}

void print_board(char** board, int width, int height){
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (board[i][j] == 0)
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

int count_neighbours(char** board, int y, int x, int width, int height){
    int count = 0;
    for (int yOff = -1; yOff < 2; yOff++){
        for (int xOff = -1; xOff < 2; xOff++){
            if (xOff == yOff && xOff == 0){
                continue;
            }
            
            count += board[(y+yOff)%height][(x+xOff)%width];
        }
    }
    return count;
}

void life_calculate_next_generation(char** new_board, char** old_board, int width, int height){
    for(int i = height; i < 2*height; i++){
        for(int j = width; j < 2*width; j++){
            int neighbours = count_neighbours(old_board, i, j, width, height);
            
            // if alive cell
            if (old_board[i%height][j%width] == 1)
            {
                if (neighbours < 2 || neighbours > 3){
                    new_board[i%height][j%width] = 0;
                    continue;
                }
                new_board[i%height][j%width] = 1;
                continue;
            }
            
            // if dead cell
            if (neighbours == 3){
                new_board[i%height][j%width] = 1;
                continue;
            }
            new_board[i%height][j%width] = 0;                    
        }
    }
}


void run_life(int width, int height, int length){
    srand(time(NULL));

    // game of life consists of a 2d array of positions filled with a number of cells in alive or dead 'states'
    // these will be denoted with a value of 0 or 1
    char** current_board = NULL;
    char** previous_board = NULL;

    current_board = (char**)malloc(sizeof(char*)*height);

    if (current_board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return;
    }
    previous_board = (char**)malloc(sizeof(char*)*height);

    if (previous_board == NULL){
        printf("\nmemory allocation issue\naborting...\n");
        return;
    }

    if (initialize_board(current_board, width, height) == 1)
    {
        printf("bad");
        return;
    }
    if (initialize_board(previous_board, width, height) == 1)
    {
        printf("bad");
        return;
    }

    struct timespec remaining, request = { 0, 50000000 };

    system("clear");
    print_board(current_board, width, height);
    nanosleep(&request, &remaining);
    if (length < 0)
    {
        length = __INT32_MAX__;
    }
    
    for(int i = 0; i < length; i++){
        system("clear");
        char** temp = previous_board;
        previous_board = current_board;
        current_board = temp;
        life_calculate_next_generation(current_board, previous_board, width, height);
        print_board(current_board, width, height);
        nanosleep(&request, &remaining);
    }

    free(current_board);
    free(previous_board);
}