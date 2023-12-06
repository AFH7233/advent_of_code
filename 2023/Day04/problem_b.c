#include <stdio.h>
#include <stdlib.h>


#define MAX_ARRAY 300

const size_t MAX_STRING_LENGTH = 200;
const size_t MAX_LINE = 1000;

size_t str_length(char line[1], size_t max_string_szie);
int get_points(char* line, size_t length);
int compare(int* a, int* b);

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
    int cards[MAX_ARRAY];
    for(size_t i=0; i<MAX_ARRAY; i++){
        cards[i] = 1;
    }
    size_t index_cards = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        size_t length = str_length(line, MAX_STRING_LENGTH);
        int points = get_points(line, length);
        int num_cards = cards[index_cards];
        while(num_cards>0){
            size_t index_next_cards = index_cards + 1;
            int points_next_cards = points;
            while(points_next_cards > 0 && index_next_cards < MAX_ARRAY){
                cards[index_next_cards] += 1;
                index_next_cards++;
                points_next_cards--;
            }
            num_cards--;
        }

        index_cards++;
    }

    int sum = 0;
    for(size_t i=0; i<index_cards; i++){
        sum += cards[i];
    }
    printf("%d\n", sum);
    fclose(file);
    free(line);
    return EXIT_SUCCESS;
}

int get_points(char* line, size_t length){
    int cards[MAX_ARRAY] = {-1};
    for(size_t i=0; i<MAX_ARRAY; i++){
        cards[i] = -1;
    }
    int games[MAX_ARRAY] = {-1};
    for(size_t i=0; i<MAX_ARRAY; i++){
        games[i] = -1;
    }
    int offset = 0;
    sscanf(line, "%*[Card 0-9]:%n", &offset);
    
    size_t index_game_separator = 0;
    for(size_t i=offset; i<length; i++){
        if(line[i] == '|'){
            index_game_separator = i;
            break;
        }
    }

    size_t index_line = offset;
    size_t index_cards = 0;
    while(index_line < index_game_separator){
        int chars_read = 0;
        int number = 0;
        sscanf(&line[index_line],"%d%n", &number, &chars_read);
        index_line += chars_read + 1;
        cards[index_cards] = number;
        index_cards++;
    }

    index_line = index_game_separator + 1;
    size_t index_game = 0;
    while(index_line < length){
        int chars_read = 0;
        int number = 0;
        sscanf(&line[index_line],"%d%n", &number, &chars_read);
        index_line += chars_read + 1;
        games[index_game] = number;
        index_game++;
    }

    int cards_length = 0;
    for(size_t i=0; i<MAX_ARRAY; i++){
        if(cards[i] == -1){
            cards_length = i;
            break;
        }
    }

    int games_length = 0;
    for(size_t i=0; i<MAX_ARRAY; i++){
        if(games[i] == -1){
            games_length = i;
            break;
        }
    }

    qsort(cards, cards_length, sizeof(int), (int (*) (const void*, const void*))compare);
    int sum = 0;
    for(size_t i=0; i<games_length; i++){
        int* card = bsearch(&games[i], cards, cards_length, sizeof(int), (int (*) (const void*, const void*))compare);
        if(card != NULL){
            sum += 1;
        } 
    }

    return sum;
}

int compare(int* a, int* b){
    return *a - *b;
}

size_t str_length(char line[1], size_t max_string_length){
    size_t count = 0;
    while(line[count] != '\0' && count < max_string_length){
        count++;
    }
    return count;
}
