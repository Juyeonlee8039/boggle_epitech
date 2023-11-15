#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// An invalid grid index
#define INVALID -1

// Functions from converting beween a 2D grid and a flattened 1D grid.

int get_x(int grid_index, int size){
    return grid_index % size;
}
int get_y(int grid_index, int size){
    return grid_index / size;
}
int get_index(int x, int y, int size){
    return y*size + x;
}

int in_bounds(int x, int y, int size){
    return 0 <= x && x < size && 0 <= y && y < size;
}

// Finds the adjacent grid cells
void adjacent(int grid_index, int size, int buf[4]){
    int x = get_x(grid_index, size);
    int y = get_y(grid_index, size);
    buf[0] = in_bounds(x - 1, y, size) ? get_index(x - 1, y, size) : INVALID;
    buf[1] = in_bounds(x + 1, y, size) ? get_index(x + 1, y, size) : INVALID;
    buf[2] = in_bounds(x, y - 1, size) ? get_index(x, y - 1, size) : INVALID;
    buf[3] = in_bounds(x, y + 1, size) ? get_index(x, y + 1, size) : INVALID;
}

// Checks if the path contains the grid index
int path_contains(int* path, int word_index, int grid_index){
    for (int i = 0; i < word_index; i++){
        if (path[i] == grid_index){
            return 1;
        }
    }
    return 0;
}

// DFS for the word from an index in the grid
int search(char* grid, int size, int grid_index, char* word, int word_size, int word_index, int* path){
    // The grid index is the position in the grid

    // Check if all letters in the word have been placed
    if (word_size == word_index){
        return 1;
    }

    // If not all the words have been placed, then check if the current letter can be placed in the current grid index
    if (grid[grid_index] != word[word_index]){
        return 0;
    }
    // Update the path
    path[word_index] = grid_index;

    // Get the adjencent indices to the current grid index
    int adj[4];
    adjacent(grid_index, size, adj);

    for (int i = 0; i < 4; i++){
        // Next possible position to place the next letter
        int next_grid_index = adj[i];
        // Check if the position (i.e, grid index) is valid and does not already have a letter on it (path_contains)
        if (next_grid_index == INVALID || path_contains(path, word_index, next_grid_index)){
            continue;
        }
        // Recursively search
        if (search(grid, size, next_grid_index, word, word_size, word_index+1, path)){
            return 1;
        }
    }
    return 0;
}

// Search from every posible starting grid index
int word_lookup(char* grid, int size, char* word){
    int word_len = strlen(word);
    // Check every possible starting position for the search
    for (int i = 0; i < size*size; i++){
        int path[word_len];
        if (search(grid, size, i, word, word_len, 0, path)){
            // Make the word found uppercase in the grid
            for (int j = 0; j < word_len; j++){
                grid[path[j]] = toupper(grid[path[j]]);
            }
            return 1;
        }
    }
    return 0;
}

void print_grid(char* grid, int size)
{
    // Print the top grid edge made of "+"
    for (int i = 0; i < 2*(size)+3; i++){
        printf("%s", "+");
    }
    
    // New line
    printf("%s", "\n");

    // Print the grid with "|" at the edges
    for (int y = 0; y < size; y++){
        printf("%s", "|");
        for (int x = 0; x < size; x++){
            printf(" %c", (grid+size*y)[x]);
        }
        printf("%s", " |\n");
    }

    // Print the bottom grid edge made of "+"
    for (int i = 0; i < 2*(size)+3; i++){
        printf("%s", "+");
    }

    // New line
    printf("%s", "\n");
}

void stringToLower(char *str) {
    // Iterate through each character in the string
    while (*str) {
        // Convert the character to uppercase
        *str = tolower((unsigned char)*str);
        // Move to the next character
        str++;
    }
}

void reset_grid(char *grid){
    stringToLower(grid);
}

int run(char *grid, int size, char *word){
    // we can check if the word is in the grid 
    if (!word_lookup(grid, size, word)){
        // Word is not found
        print_grid(grid, size);
        printf("The word \"%s\" is not in the grid.\n", word);
        reset_grid(grid);
        return 84;
    }
    else{
        // Word is found
        print_grid(grid, size);
        reset_grid(grid);
        return 0;
    }
}

int main(int argc, char *argv[]) {
    // Variable to store the option character
    int opt;

    // Default values for arguments
    char *grid = NULL;
    int size = 4;
    char *word = NULL;

    // Loop through command line options using getopt
    while ((opt = getopt(argc, argv, "g:s:w:")) != -1) {
        switch (opt) {
            case 's':
                // Convert the string argument to an integer for size (the conversion is done using atoi)
                size = atoi(optarg);
                break;
            case 'w':
                // Store the string argument for word
                word = optarg;
                break;
            case 'g':
                // Store the string argument for grid
                grid = optarg;
                break;
            default:
                // Display usage information for invalid options
                printf("Invalid options.\n");
                return 84;
        }
    }

    // We have now read the program arguments. 
    // Next we want to check if they are valid.

    // It is mandatory that the grid is specified. This means it cannot be NULL
    if (grid == NULL){
        printf("Grid not specified.\n");
        return 84;
    }

    // Check if the grid contains the correct number of characters
    if (strlen(grid) != size*size){
        printf("Grid does not contain the correct number of characters.\n");
        return 84;
    }

    // If the word has not specified as an optional argument (i.e., word is still NULL),
    // then the word must be read as input when the program is running
    if (word == NULL){
        while (1){
            char word_scan[10000];
            scanf("%10000s", word_scan);
            run(grid, size, word_scan);
        }
    }else{
        return run(grid, size, word);
    }

    // Print the arguments
    //printf("GRID: %s, SIZE: %d, WORD: %s\n", grid, size, word);

    return 0;
}