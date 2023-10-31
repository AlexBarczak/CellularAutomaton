#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h> 
#include <time.h>

#define SUCCESS 100;
#define INVALID_INPUT_PARAMETER 101;
#define FILE_ERROR 102;

char* init_generation(int size) {
    char* gen = (char*)calloc(size, size*sizeof(char));
    if (gen == NULL) {
        return NULL;
    }
    return gen;
}

int reset_generation(char* gen, int columns){
    if(gen == NULL){
        return INVALID_INPUT_PARAMETER;
    }
    for (int i = 0; i < columns; i++){
        gen[i] = 0;
    }
    return SUCCESS;
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
    // maybe an input error check
    int remainder; 
    int leftover = decimal;
    // prob change so it works for bigger nums
    for(int i = 7;i>-1;i--){
        // find value of byte starting with rightmost
        remainder = leftover%2;
        // add value of byte to string
        binary[i]=remainder;
        // get rid of that byte as we have processed it
        leftover = leftover/2;
    }
    return SUCCESS;
}

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
            // increase power of 2 by 1 power of 2
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

int get_int(char variable[]){
    int user_input=-1;
    while(user_input<0){
        printf("%s \n", variable);
        scanf("%d", &user_input);
        while(getchar()!= '\n');
        if(user_input<0){
            printf("Invalid input, try again\n");
        }
    }
}

char* resize_generation(char* gen, int size) {
    gen = (char*)realloc(gen, size*(sizeof(char)));
    if (gen == NULL) {
        return NULL;
    }
    return gen;
}

char* get_user_gen(int columns){
    char* user_input = (char*)malloc((columns+1)*sizeof(char));
    if(user_input==NULL){
        return NULL;
    }
    fgets(user_input, (columns+1), stdin);
    while(getchar()!= '\n');
    char* user_input_nums = (char*)calloc(columns, columns*sizeof(char));
    if(user_input_nums==NULL){
        return NULL;
    }
    for(int i = 0;i<columns;i++){
        if(user_input[i] == '#'){
            user_input_nums[i] = 1;
        }
    }
    free(user_input);
    return user_input_nums;
}

int main()
{
    int rule;
    int columns=100; 
    int rows=100;
    int print;
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

                // make this a funtion since its ugly in here
                print = get_int("Would you like to print the results to a file? 1=Y, 2+ = N");
                if(print == 1){
                    run_and_print(current_gen, last_gen, columns, rows, rule);
                }
                else{
                    run(current_gen, last_gen, columns, rows, rule);
                }
                reset_generation(last_gen, columns);
                reset_generation(current_gen, columns);
                break;
            case 2:
                // from https://www.geeksforgeeks.org/generating-random-number-range-c/
                rule = (rand()%256); 
                random_fill_generation(current_gen, columns);

                print = get_int("Would you like to print the results to a file? 1=Y, 2+ = N");
                if(print == 1){
                    run_and_print(current_gen, last_gen, columns, rows, rule);
                }
                else{
                    run(current_gen, last_gen, columns, rows, rule);
                }
                reset_generation(last_gen, columns);
                reset_generation(current_gen, columns);
                break;
            case 3:
                columns = get_int("Enter desired number of cells per generation");
                last_gen = resize_generation(last_gen, columns);
                reset_generation(last_gen, columns);
                rows = get_int("Enter desired number of generations to run");
                rule = (get_int("Enter desired rule to be adhered to"))%256;
                printf("Enter desired seed generation in #s and blank spaces (enter 2 for stock seed generation) \n");
                char* user_input = get_user_gen(columns);
                if(user_input == NULL){
                    return INVALID_INPUT_PARAMETER;
                }
                // add the bit to make it stock

                print = get_int("Would you like to print the results to a file? 1=Y, 2+=N");
                if(print == 1){
                    run_and_print(user_input, last_gen, columns, rows, rule);
                }
                else{
                    run(user_input, last_gen, columns, rows, rule);
                }
                free(user_input);
                columns = 100;
                rows = 100;
                last_gen = resize_generation(last_gen, columns);
                reset_generation(last_gen, columns);
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