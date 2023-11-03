#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int columns = 120;
int rows = 120;

typedef struct {
    char* cells;
} Area;

// making rule 110 for start
void calculate_next_generation(Area *previous, Area *current, char rule_byte){

    for (int i = columns; i < 2*columns; i++){
        // determine which of the eight breeding rules apply
        char state_byte = (previous->cells[(i-1)%columns] << 2) + (previous->cells[i%columns] << 1) + (previous->cells[(i+1)%columns]);

        if((rule_byte >> state_byte) & 1 == 1){
            current->cells[i%columns] = 1;
            continue;
        }
        current->cells[i%columns] = 0;
    }
}

void print_generation(Area *state){
    for (int i = 0; i < columns; i++){
        if(state->cells[i] == 1){
            printf("#");
        }else{
            printf(" ");
        }
    }
    printf("\n");
}

void append_to_file(Area *current_generation, char *filename){

}

int main(int argc, char *argv[])
{
    Area* last_generation = NULL;
    Area* current_generation = NULL;

    char rule_byte = 110;

    if (argc != 1){
        // returns zero if an invalid string is passed meaning rule zero will be run,
        // loops all values outside of our range to a value within, handles negative numbers in a wack way
        rule_byte = atoi(argv[1])%256;
        printf("arguments: %d\n", argc);
    }

    last_generation = (Area*)malloc(sizeof(Area));
    current_generation = (Area*)malloc(sizeof(Area));

    if (last_generation == NULL || current_generation == NULL){
        printf("\nnot enough memory\naborting...");
        return 1;
    }

    last_generation->cells = NULL;
    current_generation->cells = NULL;

    last_generation->cells = (char*)malloc(sizeof(char) * columns);
    current_generation->cells = (char*)malloc(sizeof(char) * columns);

    if (last_generation->cells == NULL || current_generation->cells == NULL){
        printf("\nnot enough memory\naborting...");
        return 1;
    }

    // initialise both generations to all 0
    for (int i = 0; i < columns; i++){
        last_generation->cells[i] = 0;
        current_generation->cells[i]= 0;
    }

    //starting state sets the center to a 1
    current_generation->cells[columns/2] = 1;

    struct timespec remaining, request = { 0, 16000000 };

    print_generation(current_generation);
    nanosleep(&request, &remaining);
    for (int i = 0; i < rows-1; i++){
        Area *temp = last_generation;
        last_generation = current_generation;
        current_generation = temp;
        calculate_next_generation(last_generation, current_generation, rule_byte);
        print_generation(current_generation);
        append_to_file(current_generation, "output");
        nanosleep(&request, &remaining);
    }

    free(last_generation->cells);
    free(current_generation->cells);
    free(last_generation);
    free(current_generation);
}
