#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLUMNS 120
#define ROWS 200

typedef struct {
    char cells[COLUMNS];
} Area;

// making rule 110 for start
void calculate_next_generation(Area *previous, Area *current, char rule_byte){

    for (int i = 0; i < COLUMNS; i++){
        // determine which of the eight breeding rules apply
        char state_byte = (previous->cells[(i-1)%COLUMNS] << 2) + (previous->cells[i] << 1) + (previous->cells[(i+1)%COLUMNS]);

        if((rule_byte >> state_byte) & 1 == 1){
            current->cells[i] = 1;
            continue;
        }
        current->cells[i] = 0;
    }
}

void print_generation(Area *state){
    for (int i = 0; i < COLUMNS; i++){
	if(state->cells[i] == 1){
	    printf("#");
	}else{
	    printf(",");
	}
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    Area* last_generation;
    Area* current_generation;

    char rule_byte = 110;

    if (argc != 1){
        rule_byte = atoi(argv[1])%256;
        printf("arguments: %d\n", argc);
    }

    last_generation = malloc(sizeof(Area));
    current_generation = malloc(sizeof(Area));

    // initialise both generations to all 0
    for (int i = 0; i < COLUMNS; i++){
        last_generation->cells[i] = 0;
        current_generation->cells[i]= 0;
    }

    //starting state sets the center to a 1
    current_generation->cells[COLUMNS/2] = 1;

    struct timespec remaining, request = { 0, 16000000 };

    print_generation(current_generation);
    nanosleep(&request, &remaining);
    for (int i = 0; i < ROWS-1; i++){
        Area *temp = last_generation;
        last_generation = current_generation;
        current_generation = temp;
        calculate_next_generation(last_generation, current_generation, rule_byte);
        print_generation(current_generation);
        nanosleep(&request, &remaining);
    }
}
