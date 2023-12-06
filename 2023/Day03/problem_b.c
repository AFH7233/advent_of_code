#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct gear {
    int num_a;
    int num_b;
} gear;

const int EMPTY = -1;
const int GEAR =  -2;
const int UNKNOWN = -3;
const size_t MAX_STRING_LENGTH = 200;
const size_t MAX_LINE = 1000;

size_t str_length(char line[1], size_t max_string_szie);
char* fill_row(char* line, size_t length, char* row);
void explore_gear(char** tools, int** types, int i, int j, int max_i, int max_j, int num_gear);
void explore_number(char** tools, int** types, int i, int j, int max_i, int max_j, int num_gear);

int main(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr, "Necesito un input prro\n");
        exit(EXIT_FAILURE);
    }

    char* file_name = argv[1];
    FILE *file = fopen(file_name, "r");
    if(file == NULL){
        fprintf(stderr, "No pude leer el archivo\n");
        exit(EXIT_FAILURE);
    }

    char* line = calloc(MAX_LINE, sizeof(char));
    int max = 0;
    int count = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        size_t length = str_length(line, MAX_STRING_LENGTH);
        max = max < length? length : max;
        count++;
    }

    char** tools = calloc(count, sizeof(char*));     
    int** types = calloc(count, sizeof(int*)); 

    for(size_t i = 0; i < count; i++){
        tools[i] = calloc(max, sizeof(char));
        types[i] = calloc(max, sizeof(int));
        for(size_t j = 0; j < max; j++ ){
            types[i][j] = UNKNOWN;
        }
    }

    fseek(file, 0, SEEK_SET);
    int index_row = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        tools[index_row] = fill_row(line, max, tools[index_row]);
        tools[index_row][max] = '\0';
        index_row++;
    }

    int num_gear = 0;
    for(size_t i = 0; i < count; i++){
        for(size_t j=0; j < max; j++){
            if(types[i][j] != UNKNOWN){
                continue;
            }
            if(tools[i][j] == '.'){
                types[i][j] = EMPTY;
            } else if (tools[i][j] == '*'){
                explore_gear(tools, types, i, j, count, max, num_gear);
                num_gear++;
            }
        }
    }
    gear* arr_gear = calloc(num_gear, sizeof(gear));
    for(size_t i=0; i<num_gear; i++){
        arr_gear[i] = (gear){
            .num_a=-1,
            .num_b=-1,
        };
    }


    for(size_t i = 0; i < count; i++){
        for(size_t j=0; j < max; j++){
            if(types[i][j] <= EMPTY){
                tools[i][j] = ' ';
            }
        }
    }

    for(size_t i = 0; i < count; i++){
        int index = 0;
        while(index < max){
            int count_chars;
            int number;
            int successfull = sscanf(&tools[i][index], "%d%n", &number, &count_chars);
            if(successfull == 1){
                int index_gear = types[i][index + count_chars - 1];
                if(arr_gear[index_gear].num_a > 0){
                    arr_gear[index_gear].num_b = number;
                } else {
                    arr_gear[index_gear].num_a = number;
                }
                index += (count_chars + 1);
            } else {
                break;
            }
        }
    }

    long sum = 0;
    for(size_t i = 0; i<num_gear; i++){
        if(arr_gear[i].num_a > -1 && arr_gear[i].num_b > -1){
            sum += (arr_gear[i].num_a * arr_gear[i].num_b);
        }
    }
    printf("%ld\n", sum);

    fclose(file);
    free(line);
    for(size_t i = 0; i < (max); i++){
        free(tools[i]);
        tools[i] = NULL;
    }
    free(arr_gear);
    free(tools);
    return EXIT_SUCCESS;
}

void  explore_gear(char** tools, int** types, int i, int j, int max_i, int max_j, int num_gear){
    if(i < 0 || i >= max_i){
        return;
    } else if(j < 0 || j >= max_j){
        return;
    } else if(types[i][j] != UNKNOWN){
        return;
    }
    types[i][j] = GEAR;
    explore_number(tools, types, i-1, j-1, max_i, max_j, num_gear);
    explore_number(tools, types, i-1, j, max_i, max_j, num_gear);
    explore_number(tools, types, i-1, j+1, max_i, max_j, num_gear);
    explore_number(tools, types, i, j-1, max_i, max_j, num_gear);
    explore_number(tools, types, i, j+1, max_i, max_j, num_gear);
    explore_number(tools, types, i+1, j-1, max_i, max_j, num_gear);
    explore_number(tools, types, i+1, j, max_i, max_j, num_gear);
    explore_number(tools, types, i+1, j+1, max_i, max_j, num_gear);
}

void  explore_number(char** tools, int** types, int i, int j, int max_i, int max_j, int num_gear){
    if(i < 0 || i >= max_i){
        return;
    } else if(j < 0 || j >= max_j){
        return;
    } else if(types[i][j] != UNKNOWN){
        return;
    } else if (!( '0' <= tools[i][j] && tools[i][j] <= '9')){
        return;
    }
    types[i][j] = num_gear;
    explore_number(tools, types, i, j-1, max_i, max_j, num_gear);
    explore_number(tools, types, i, j+1, max_i, max_j, num_gear);
}

char* fill_row(char* line, size_t length, char* row){
    for(size_t i = 0; i < length; i++){
        row[i] = line[i];
    }
    return row;
} 

size_t str_length(char line[1], size_t max_string_length){
    size_t count = 0;
    while(line[count] != '\0' && count < max_string_length){
        count++;
    }
    return count;
}
