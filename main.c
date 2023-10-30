#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> 
#include <time.h>

#define SUCCESS 100;
#define INVALID_INPUT_PARAMETER 101;
#define FILE_ERROR 102;

char* init_generation(int size) {
    if (size < 0){
        return NULL;
    }

    char* gen = (char*)calloc(size, size*sizeof(char));
    if (gen == NULL) {
        return NULL;
    }
    return gen;
}

void reset_generation(char* gen, int columns){
    for (int i = 0; i < columns; i++){
        gen[i] = 0;
    }
}


int random_fill_generation(char* gen, int columns){
    if(gen == NULL){
        return INVALID_INPUT_PARAMETER;
    }
    else{
        for(int i = 0; i <columns;i++)
        gen[i] = (rand() % (2)); 
    }
    return SUCCESS;
}

void calculate_next_generation(char* previous, char* current, char rule_byte, int columns){
     for (int i = columns; i < 2*columns; i++){
        // determine which of the eight breeding rules apply
        char state_byte = (previous[(i-1)%columns] << 2) + (previous[i%columns] << 1) + (previous[(i+1)%columns]);

        if(((rule_byte >> state_byte) & 1) == 1){
            current[i%columns] = 1;
            continue;
        }
        current[i%columns] = 0;
    }
}

void print_generation(char* state, int columns){
    for (int i = 0; i < columns; i++){
	if(state[i] == 1){
	    printf("#");
	}else{
	    printf(" ");
	}
    }
    printf("\n");
}

void print_generation_to_file(char* state, int columns, FILE *fp){
    for (int i = 0; i < columns; i++){
	    if(state[i] == 1){
	        fprintf(fp, "#");
	    }else{
	        fprintf(fp, " ");
	    }
    }
    fprintf(fp, "\n");
}

int decimal_to_binary(int decimal, char* binary){
    int remainder; 
    int leftover = decimal;
    for(int i = 8;i>0;i--){
        // find value of byte starting with leftmost
        remainder = leftover%2;
        // add value of byte to string
        binary[i]=remainder;
        // get rid of that byte as we have processed it
        leftover = remainder/2;
    }
    return SUCCESS;
}

// takes string as leading 0's change input if it's an int e.g 00101 = 65 whereas atoi would use 101
int binary_to_decimal(char num[]){
    if(isdigit(num[0])){
        int binary = atoi(num);
        int decimal = 0;
        int power_of_two=1;
        while(binary!=0){
            // get rightmost value
            int last_byte = binary%10;
            // get rid of rightmost value now that we have it
            binary = binary/10;
            // do rightmost value multiplied by corresponding power of 2 and add to total
            decimal = decimal + (last_byte*power_of_two);
            // increase power of 2 by 1 power
            power_of_two = power_of_two << 1;
        }
    }
    else
        return INVALID_INPUT_PARAMETER;
    return SUCCESS;
}

int run(char *current_gen, char *last_gen, int columns, int rows, int rule){
    struct timespec remaining, request = { 0, 16000000 };
    print_generation(current_gen, columns);
    for (int i = 0; i < rows-1; i++){
        char *temp = last_gen;
        last_gen = current_gen;
        current_gen = temp;
        calculate_next_generation(last_gen, current_gen, rule, columns);
        print_generation(current_gen, columns);
        nanosleep(&request, &remaining);
    }
    return SUCCESS;
}

int run_and_print(char *current_gen, char *last_gen, int columns, int rows, int rule){
    struct timespec remaining, request = { 0, 16000000 };
    FILE *fp = NULL;
    // could let them choose file name
    fp = fopen("cellular-automata.txt", "w");
    if(fp!=NULL){
        print_generation(current_gen, columns);
        print_generation_to_file(current_gen, columns, fp);
        for (int i = 0; i < rows-1; i++){
            char *temp = last_gen;
            last_gen = current_gen;
            current_gen = temp;
            calculate_next_generation(last_gen, current_gen, rule, columns);
            print_generation(current_gen, columns);
            print_generation_to_file(current_gen, columns, fp);
            nanosleep(&request, &remaining);
        }
    }
    else{
        return FILE_ERROR;
    }
    fclose(fp);
    return SUCCESS;
}

int main()
{
    int rule;
    int columns=100; 
    int rows=100;
    int print=0;
    char* last_gen = init_generation(columns);
    char* current_gen = init_generation(columns);
    if(current_gen == NULL || last_gen == NULL)
    {
        return INVALID_INPUT_PARAMETER;
    }
    srand(time(NULL));

    int choice;

    while (1) {
        printf("---- Menu ----\n");
        printf("1. Print a stock 1D Cellular automaton\n");
        printf("2. Print a random 1D cellular automaton\n");
        printf("3. Print your choice of 1D cellular automaton\n");
        printf("4. Quit\n");
        printf("Enter your choice:\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                rule = 110; 
                current_gen[columns/2] = 1;
                printf("Would you like to print the results to a file? 1=Y, other = N\n");
                scanf("%d", &print);
                if(print == 1){
                    run_and_print(current_gen, last_gen, columns, rows, rule);
                }
                else{
                    run(current_gen, last_gen, columns, rows, rule);
                }
                while(getchar()!= '\n');
                reset_generation(last_gen, columns);
                reset_generation(current_gen, columns);
                break;
            case 2:
                // from https://www.geeksforgeeks.org/generating-random-number-range-c/
                rule = (rand() % (256)); 
                random_fill_generation(current_gen, columns);
                printf("Would you like to print the results to a file? 1=Y, other = N\n");
                scanf("%d", &print);
                if(print == 1){
                    run_and_print(current_gen, last_gen, columns, rows, rule);
                }
                else{
                    run(current_gen, last_gen, columns, rows, rule);
                }
                while(getchar()!= '\n');
                reset_generation(last_gen, columns);
                reset_generation(current_gen, columns);
                break;
            case 3:
                // need input validation
                printf("Enter desired number of cells in a generation \n");
                scanf("%d", &columns);
                current_gen = realloc(current_gen, columns);
                last_gen = realloc(last_gen, columns);
                // check NULL
                reset_generation(last_gen, columns);
                while(getchar()!= '\n');
                // need input validation
                printf("Enter desired number of generations to run\n");
                scanf("%d", &rows);
                while(getchar()!= '\n');
                // need input validation
                printf("Enter desired rule number in decimal to be adhered to \n");
                scanf("%d", &rule);
                while(getchar()!= '\n');
                // need everything
                printf("Enter desired seed generation in 1's and 0's without spaces (enter 2 for stock seed generation) \n");
                char* user_input = (char*)malloc(columns*sizeof(char));
                // check NULL
                fgets(user_input, columns, stdin);
                printf("Would you like to print the results to a file? 1=Y, other = N\n");
                scanf("%d", &print);
                if(print == 1){
                    run_and_print(user_input, last_gen, columns, rows, rule);
                }
                else{
                    run(user_input, last_gen, columns, rows, rule);
                }
                while(getchar()!= '\n');
                free(user_input);
                columns = 100;
                rows = 100;
                current_gen = realloc(current_gen, columns);
                last_gen = realloc(last_gen, columns);
                // check NULL
                reset_generation(last_gen, columns);
                reset_generation(current_gen, columns);
                break;
            case 4:
                printf("Goodbye!\n");
                return SUCCESS;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                while(getchar()!= '\n');
        }
    }
    free(last_gen);
    free(current_gen);
    return SUCCESS;
}
