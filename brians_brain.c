#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "life.h"

void brians_brain_calculate_next_generation(char** new_board, char** old_board, int width, int height){
    for(int i = height; i < 2*height; i++){
        for(int j = width; j < 2*width; j++){
            int neighbours = count_neighbours(old_board, i, j, width, height);
            
            // if alive cell
            if (old_board[i%height][j%width] == 1)
            {
                new_board[i%height][j%width] = 2;
                continue;
            }
            if (old_board[i%height][j%width] == 2){
                new_board[i%height][j%width] = 0;
                continue;
            }
            // if dead cell
            if (neighbours == 2){
                new_board[i%height][j%width] = 1;
                continue;
            }
            new_board[i%height][j%width] = 0;                    
        }
    }
}



void run_brians_brain(int width, int height, int length){
    srand(time(NULL));

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
        printf("\nmemory allocation issue\naborting...\n");
        return;
    }
    if (initialize_board(previous_board, width, height) == 1)
    {
        printf("\nmemory allocation issue\naborting...\n");
        return;
    }

    struct timespec remaining, request = { 0, 80000000 };

    system("clear");
    print_board(current_board, width, height);
    nanosleep(&request, &remaining);    
    for(int i = 0; i < length; i++){
        system("clear");
        char** temp = previous_board;
        previous_board = current_board;
        current_board = temp;
        brians_brain_calculate_next_generation(current_board, previous_board, width, height);
        print_board(current_board, width, height);
        nanosleep(&request, &remaining);
    }

    free(current_board);
    free(previous_board);
}