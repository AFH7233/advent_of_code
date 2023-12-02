#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const size_t MAX_STRING_LENGTH = 200;
const size_t MAX_LINE = 1000;
const size_t MAX_LOOP = 10;

const size_t MAX_RED_CUBES = 12;
const size_t MAX_GREEN_CUBES = 13;
const size_t MAX_BLUE_CUBES = 14;

enum color{
    RED,
    GREEN,
    BLUE,
    NO_COLOR
};

typedef struct game_set{
    int quantity;
    enum color color;
} game_set;

typedef struct arr_game_set{
    size_t size;
    game_set elements[];
} arr_game_set;

size_t str_length(char line[1], size_t max_string_length);
arr_game_set* parse_game(char line[1], size_t length);
int find_max(arr_game_set* game, enum color color);

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
    int sum = 0;
    int id = 1;
    while(fgets(line, MAX_LINE, file) != NULL){
        size_t length = str_length(line, MAX_STRING_LENGTH);
        arr_game_set* game = parse_game(line, length);
        int max_red = find_max(game, RED);
        int max_green = find_max(game, GREEN);
        int max_blue = find_max(game, BLUE);
        bool is_game_possible = (max_red <= MAX_RED_CUBES) & 
                            (max_green <= MAX_GREEN_CUBES) & 
                            (max_blue <= MAX_BLUE_CUBES);
        sum += (is_game_possible? id:0);
        //printf("%d, %d, %d\n", max_red, max_green, max_blue);
        id++;
        free(game);
    }
    printf("%d\n", sum);
    fclose(file);
    free(line);
    return EXIT_SUCCESS;
}

int find_max(arr_game_set* game, enum color color){
    int max = 0;
    for(size_t i=0; i<game->size; i++){
        if(game->elements[i].color == color){
            int quantity = game->elements[i].quantity;
            if(max < quantity){
                max = quantity;
            }
        }
    }
    return max;
}

arr_game_set* parse_game(char line[1], size_t length){

    size_t sets_start = 0;
    size_t number_of_sets = 1;
    for(size_t i=0; i<length; i++){
        if(line[i] == ':'){
            sets_start = i;
        } else if(line[i]  == ';' || line[i] == ','){
            number_of_sets++;
        }
    }
    arr_game_set* sets = malloc(sizeof(arr_game_set) + number_of_sets * sizeof(game_set));
    sets->size = number_of_sets;
    size_t index = sets_start + 1;
    size_t count = 0;
    while(index<length && count < number_of_sets){
        int chars_consumed = 0;
        int quantity = 0;
        char color_str[10];  
        sscanf(&line[index], " %d %[a-z]%n", &quantity, color_str, &chars_consumed);
        index += (chars_consumed + 1);
        char initial = color_str[0];
        enum color color;
        switch(initial) {
            case 'r':
                color = RED;
                break;
            case 'b':
                color = BLUE;
                break;
            case 'g':
                color = GREEN;
                break;
            default:
                color = NO_COLOR;
                break;
        }
        if(color == NO_COLOR){
            fprintf(stderr, "Color invalido algo salio mal\n");
            exit(EXIT_FAILURE);          
        }
        game_set set = {
            .quantity = quantity,
            .color = color
        };
        //printf("%d %d\n", set.quantity, set.color);
        sets->elements[count] = set;
        count++;
    }

    return sets;
}

size_t str_length(char line[1], size_t max_string_length){
    size_t count = 0;
    while(line[count] != '\0' && count < max_string_length){
        count++;
    }
    return count;
}
