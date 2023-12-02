#include <stdio.h>
#include <stdlib.h>

const size_t MAX_STRING_SIZE = 200;
const size_t MAX_LINE = 1000;

size_t str_size(char line[1], size_t max_string_szie);
int get_number(char line[1], size_t size);

int main(int argc, char *argv[]){
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
    long sum = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        size_t size = str_size(line, MAX_STRING_SIZE);
        int number = get_number(line, size);
        sum += number;
    }
    printf("%ld\n", sum);
    fclose(file);
    free(line);
    return EXIT_SUCCESS;
}

int get_number(char line[1], size_t size){
    int first = 0;
    for(size_t i=0; i<size; i++){
        if('0' <= line[i] && line[i]<='9'){
            first = line[i] - '0';
            break;
        }
    }

    int second = 0;
    for(size_t i=size-1; i>=0; i--){
        if('0' <= line[i] && line[i]<='9'){
            second = line[i] - '0';
            break;
        }
    }

    return first*10 + second;
}

size_t str_size(char line[1], size_t max_string_szie){
    size_t count = 0;
    while(line[count] != '\0' && count < max_string_szie){
        count++;
    }
    return count;
}
