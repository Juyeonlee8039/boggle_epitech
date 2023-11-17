#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#define MASK_CHAR_EMPTY '.'
#define MASK_CHAR_FOUND '#'

/**
 * @brief struct containing the grid information
*/
typedef struct grid_s {
    char** grid;
    char** mask;
    size_t size;
} grid_t;

/**
 * @brief returns a boolean value, checking whether a point is inside the bounds of the grid of side size
 * @param i x coordinate
 * @param j y coordinate
 * @param size grid size
*/
int is_inside_grid(size_t i, size_t j, size_t size) {
    if (i >= size || j >= size) {
        return 0;
    }

    return 1;
}

/**
 * @brief prints the grid, if mask is specified, will highlight the word, otherwise every character will be printed lowercase
 * @param grid pointer to grid struct
*/
void print_grid(grid_t* grid) {
    for (size_t i = 0; i < grid->size + 4; i++) {
        printf("+");
    }
    printf("\n");

    for (size_t i = 0; i < grid->size; i++) {
        printf("| ");
        for (size_t j = 0; j < grid->size; j++) {
            printf("%c", grid->mask != NULL && grid->mask[i][j] == MASK_CHAR_FOUND ? toupper(grid->grid[i][j]) : grid->grid[i][j]);
        }
        printf(" |\n");
    }

    for (size_t i = 0; i < grid->size + 4; i++) {
        printf("+");
    }
    printf("\n");
}

/**
 * @brief recursively checks neighboring tiles for matching letter, returns 1 if found up to the end, 0 otherwise
 * @param i current position x coordinate
 * @param j current position y coordinate
 * @param grid pointer to the grid struct
 * @param word word at the current checked position
*/
int search_around(size_t i, size_t j, grid_t* grid, const char* word) {
    grid->mask[i][j] = MASK_CHAR_FOUND;

    if (word[0] == '\0') {
        return 1;
    }

    if (is_inside_grid(i + 1, j, grid->size) && grid->grid[i + 1][j] == word[0] && grid->mask[i + 1][j] != MASK_CHAR_FOUND) {
        if (search_around(i + 1, j, grid, &word[1])) {
            return 1;
        }
    }

    if (is_inside_grid(i, j + 1, grid->size) && grid->grid[i][j + 1] == word[0] && grid->mask[i][j + 1] != MASK_CHAR_FOUND) {
        if (search_around(i, j + 1, grid, &word[1])) {
            return 1;
        }
    }

    if (is_inside_grid(i - 1, j, grid->size) && grid->grid[i - 1][j] == word[0] && grid->mask[i - 1][j] != MASK_CHAR_FOUND) {
        if (search_around(i - 1, j, grid, &word[1])) {
            return 1;
        }
    }

    if (is_inside_grid(i, j - 1, grid->size) && grid->grid[i][j - 1] == word[0] && grid->mask[i][j - 1] != MASK_CHAR_FOUND) {
        if (search_around(i, j - 1, grid, &word[1])) {
            return 1;
        }
    }

    grid->mask[i][j] = MASK_CHAR_EMPTY;

    return 0;
}

/**
 * @brief does the job, returns 1 if found, 0 otherwise
 * @param word string to be searched for in the grid
 * @param grid pointer to the grid 
*/
int word_lookup(const char* word, grid_t* grid) {
    for (size_t i = 0; i < grid->size; i++) {
        for (size_t j = 0; j < grid->size; j++) {
            if (grid->grid[i][j] == word[0]) {
                if (search_around(i, j, grid, &word[1]) == 1) {
                    print_grid(grid);
                    return 1;
                }
            }
        }
    }

    fprintf(stderr, "The word \"%s\" is not in the grid.\n", word);

    return 0;
}

/**
 * @brief check if a string is in the provided character range: "abcdefghijklmnopqrstuvwxyz"
 * @param str string to check
*/
int is_string_alpha_only(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < 'a' || str[i] > 'z') {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief check if a string is containing only numbers
 * @param str string to check
*/
int is_string_numeric_only(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief clean given string from newlines
 * @param str string to clean
*/
void clean_input(char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
        }
    }
}

/**
 * @brief main game loop
 * @param grid pointer to the grid
*/
int boggle_run(grid_t* grid) {
    int status = 0;
    char input[1024] = {0};

    print_grid(grid);

    while (1) {
        printf("> ");

        fgets(input, 1024, stdin);
        clean_input(input);

        if (strcmp(input, "0") == 0) {
            return 0;
        }

        if (input[0] != '\0') {
            status = word_lookup(input, grid);
        }

        if (status == 1) {
            return 0;
        }
    }
}

/**
 * @brief allocates memory and initializes the grid and the mask
 * @param grid grid struct
 * @param g_val value of the -g argument
*/
void initialize_grid(grid_t* grid, char* g_val) {
    grid->grid = malloc(grid->size * sizeof(char*));
    grid->mask = malloc(grid->size * sizeof(char*));
    
    for (size_t i = 0; i < grid->size; i ++) {
        grid->grid[i] = malloc(grid->size);
        grid->mask[i] = malloc(grid->size);
    }

    for (size_t i = 0; i < grid->size; i++) {
        for (size_t j = 0; j < grid->size; j++) {
            grid->grid[i][j] = g_val[i * grid->size + j];
            grid->mask[i][j] = MASK_CHAR_EMPTY;
        }
    }
}

/**
 * @brief frees the grid
 * @param grid pointer to the grid
*/
void free_grid(grid_t* grid) {
    for (size_t i = 0; i < grid->size; i ++) {
        free(grid->grid[i]);
        free(grid->mask[i]);
    }

    free(grid->grid);
    free(grid->mask);
}

/**
 * @brief boggle game
 * @param argc argument count
 * @param argv argument values (./program -g "lqnelooelbtelqte" [-s <number>] [-w <string>])
 */
int main(int argc, char* const *argv) {
    int status = 0;
    grid_t grid = {0};

    int s_opt = 0;
    int w_opt = 0;
    int g_opt = 0;
    char* s_val = NULL;
    char* w_val = NULL;
    char* g_val = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "g:s:w:")) != -1)
    {
        switch (opt) {
            case 'g': {
                g_opt = 1;
                g_val = optarg;
                break;
            }
            case 's': {
                s_opt = 1;
                s_val = optarg;
                break;
            }
            case 'w': {
                w_opt = 1;
                w_val = optarg;
                break;
            }
            default: {
                return 84;
            }
        }
    }
    
    if (g_opt == 0) {
        return 84;
    }

    if (!is_string_alpha_only(g_val)) {
        return 84;
    }

    if (s_opt) {
        if (!is_string_numeric_only(s_val)) {
            return 84;
        }

        grid.size = (size_t)atoi(s_val);
        if (grid.size == 0) {
            return 84;
        }
    }

    if (strlen(g_val) != grid.size * grid.size) {
        fprintf(stderr, "The grid does not contain the correct number of characters.");
        return 84;
    }

    initialize_grid(&grid, g_val);

    if (w_opt) {
        if (word_lookup(w_val, &grid) == 0) {
            status = 84;
        } else {
            status = 0;
        }

        goto main_exit;
    }

    status = boggle_run(&grid);

main_exit:
    free_grid(&grid);

    return status;
}