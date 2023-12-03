#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


enum cell_type {
    NUMBER,
    EMPTY,
    SYMBOL,
    UNKNOWN
};

const size_t MAX_STRING_LENGTH = 200;
const size_t MAX_LINE = 1000;

// This is enough for the problem and to save work
// on creating some growing structure.
const size_t MAX_ARRAY = 300;

size_t str_length(char line[1], size_t max_string_szie);
char* fill_row(char* line, size_t length, char* row);
void explore_symbol(char** tools, enum cell_type** types, int i, int j, int max_i, int max_j);
void explore_number(char** tools, enum cell_type** types, int i, int j, int max_i, int max_j);

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
    enum cell_type** types = calloc(count, sizeof(enum cell_type*)); 

    for(size_t i = 0; i < count; i++){
        tools[i] = calloc(max+1, sizeof(char)); //One more to make it a string.
        types[i] = calloc(max, sizeof(enum cell_type));
        for(size_t j = 0; j < max; j++ ){
            types[i][j] = UNKNOWN;
        }
        tools[i][max] = '\0';
    }

    fseek(file, 0, SEEK_SET);
    int index_row = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        tools[index_row] = fill_row(line, max, tools[index_row]);
        index_row++;
    }

    for(size_t i = 0; i < count; i++){
        for(size_t j=0; j < max; j++){
            if(types[i][j] != UNKNOWN){
                continue;
            }
            if(tools[i][j] == '.'){
                types[i][j] = EMPTY;
            } else if (!( '0' <= tools[i][j] && tools[i][j] <= '9')){
                explore_symbol(tools, types, i, j, count, max);
            }
        }
    }

    for(size_t i = 0; i < count; i++){
        for(size_t j=0; j < max; j++){
            if(types[i][j] != NUMBER){
                tools[i][j] = ' ';
            }
        }
    }

    int sum = 0;
    for(size_t i = 0; i < count; i++){
        int index = 0;
        while(index < max){
            int count_chars;
            int number;
            int successfull = sscanf(&tools[i][index], "%d%n", &number, &count_chars);
            if(successfull == 1){
                index += (count_chars + 1);
                sum += number;
            } else {
                break;
            }
        }
    }
    printf("%d\n", sum);

    fclose(file);
    free(line);
    for(size_t i = 0; i < (max); i++){
        free(tools[i]);
        tools[i] = NULL;
    }
    free(tools);
    return EXIT_SUCCESS;
}

void  explore_symbol(char** tools, enum cell_type** types, int i, int j, int max_i, int max_j){
    if(i < 0 || i >= max_i){
        return;
    } else if(j < 0 || j >= max_j){
        return;
    } else if(types[i][j] != UNKNOWN){
        return;
    }
    types[i][j] = SYMBOL;
    explore_number(tools, types, i-1, j-1, max_i, max_j);
    explore_number(tools, types, i-1, j, max_i, max_j);
    explore_number(tools, types, i-1, j+1, max_i, max_j);
    explore_number(tools, types, i, j-1, max_i, max_j);
    explore_number(tools, types, i, j+1, max_i, max_j);
    explore_number(tools, types, i+1, j-1, max_i, max_j);
    explore_number(tools, types, i+1, j, max_i, max_j);
    explore_number(tools, types, i+1, j+1, max_i, max_j);
}

void  explore_number(char** tools, enum cell_type** types, int i, int j, int max_i, int max_j){
    if(i < 0 || i >= max_i){
        return;
    } else if(j < 0 || j >= max_j){
        return;
    } else if(types[i][j] != UNKNOWN){
        return;
    } else if (!( '0' <= tools[i][j] && tools[i][j] <= '9')){
        return;
    }
    types[i][j] = NUMBER;
    explore_number(tools, types, i, j-1, max_i, max_j);
    explore_number(tools, types, i, j+1, max_i, max_j);
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
