#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const size_t MAX_STRING_LENGTH = 200;
const size_t MAX_LINE = 1000;
const size_t ALPHABET_SIZE = 26;

typedef struct trie_node {
    bool is_terminal;
    int value;
    struct trie_node* children[ALPHABET_SIZE];
} trie_node;

size_t str_length(char line[1], size_t max_string_szie);
int get_number(char line[1], size_t length, trie_node normal[1]);
trie_node* insert_word(char* word, size_t length, int value, trie_node* root);
trie_node* delete_trie(trie_node* root);
trie_node* create_trie_node(void);

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

    trie_node* root_normal = create_trie_node();
    insert_word("one", 3, 1, root_normal);
    insert_word("two", 3, 2, root_normal);
    insert_word("three", 5, 3, root_normal);
    insert_word("four", 4, 4, root_normal);
    insert_word("five", 4, 5, root_normal);
    insert_word("six", 3, 6, root_normal);
    insert_word("seven", 5, 7, root_normal);
    insert_word("eight", 5, 8, root_normal);
    insert_word("nine", 4, 9, root_normal);

    char* line = calloc(MAX_LINE, sizeof(char));
    long sum = 0;
    while(fgets(line, MAX_LINE, file) != NULL){
        size_t length = str_length(line, MAX_STRING_LENGTH);
        int number = get_number(line, length, root_normal);
        /*for(size_t i=0; i<length; i++){
            if(line[i] == '\n'){
                break;
            }
            printf("%c",line[i]);
        }
        printf(" %d\n", number);*/
        sum += number;
    }
    printf("%ld\n", sum);
    root_normal = delete_trie(root_normal);
    fclose(file);
    free(line);

    return EXIT_SUCCESS;
}

trie_node* create_trie_node(void){
    trie_node* node = malloc(sizeof(trie_node));
    if(node == NULL){
        fprintf(stderr, "No hay memoria hdtpm\n");
        exit(EXIT_FAILURE); 
    }
    for(size_t i=0; i<ALPHABET_SIZE; i++){
        node->children[i] = NULL;
    }
    node->is_terminal = false;
    node->value = -1;
    return node;
}

trie_node* delete_trie(trie_node* node){
    for(size_t i=0; i<ALPHABET_SIZE; i++){
        if(node->children[i] != NULL){
            node->children[i] = delete_trie(node->children[i]);
        }
    }
    free(node);
    return NULL;
}

trie_node* insert_word(char* word, size_t length, int value, trie_node* root){
    trie_node* current = root;
    for(size_t i=0; i<length; i++){
        char letter = word[i];
        int index = letter - 'a';
        trie_node* node = current->children[index];
        if(node == NULL){
            
            current->children[index] = create_trie_node(); 
            node = current->children[index];
        }
        current=node;
    }
    current->is_terminal=true;
    current->value=value;
    
    return root;
}

int get_number(char line[1], size_t length, trie_node normal[1]){
    int* num = calloc(length, sizeof(int));
    for(size_t i=0;i<length;i++){
        char letter = line[i];
        if('0' <= letter && letter <='9'){
            num[i] = letter - '0';
        } else {
            trie_node* current = normal;
            for(size_t j=i; j<length; j++){
                int index = line[j] - 'a';
                current = current->children[index];
                if(current == NULL){
                    num[i] = 0;
                    break;
                }
                if(current->is_terminal){
                    num[i] = current->value;
                    break;
                }
            }
        }
    }

    int first = 0;
    for(size_t i=0;i<length-1;i++){
        if(num[i] > 0){
            first = num[i];
            break;
        } 
    }

    int second = 0;
    for(size_t i=length-1;i>=0;i--){
        if(num[i] > 0){
            second = num[i];
            break;
        } 
    }
    free(num);
    return first*10 + second;
}

size_t str_length(char line[1], size_t max_string_length){
    size_t count = 0;
    while(line[count] != '\0' && count < max_string_length){
        count++;
    }
    return count;
}
