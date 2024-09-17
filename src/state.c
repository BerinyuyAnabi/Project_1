#include "state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

#define NUM_ROWS 18
#define NUM_COLS 20
#define INITIAL_BOARD_SIZE 10
#define LINE_SIZE 256

/* Task 1 */
game_state_t *create_default_state() {
    game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));
    if (state == NULL) {
        return NULL;
    }

    state->num_rows = NUM_ROWS;
    state->board = (char**) malloc(NUM_ROWS * sizeof(char*));
    if (state->board == NULL) {
        free(state);
        return NULL;
    }

    char* default_board[NUM_ROWS] = {
        "####################",
        "#                  #",
        "# d>D    *         #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "####################"
    };

    for (unsigned int i = 0; i < NUM_ROWS; ++i) {
        state->board[i] = (char*) malloc((NUM_COLS + 1) * sizeof(char));  // +1 for null terminator
        if (state->board[i] == NULL) {
            for (unsigned int j = 0; j < i; ++j) {
                free(state->board[j]);
            }
            free(state->board);
            free(state);
            return NULL;
        }
        strcpy(state->board[i], default_board[i]);
    }

    state->num_snakes = 1;
    state->snakes = (snake_t*) malloc(state->num_snakes * sizeof(snake_t));
    if (state->snakes == NULL) {
        for (unsigned int i = 0; i < NUM_ROWS; ++i) {
            free(state->board[i]);
        }
        free(state->board);
        free(state);
        return NULL;
    }

    state->snakes[0].tail_row = 2;
    state->snakes[0].tail_col = 2;
    state->snakes[0].head_row = 2;
    state->snakes[0].head_col = 4;
    state->snakes[0].live = true;

    return state;
}
// /* Task 2*/
void free_state(game_state_t* state) {
    if (state == NULL) {
        return;
    }

    // Freeing each row of the board
    for (int i = 0; i < state->num_rows; i++) {
        free(state->board[i]);
    }
    // Freeing the board itself
    free(state->board);

    // // Freeing each snake
    // for (int i = 0; i < state->num_snakes; i++) {
    //     free(state->snakes[i]);
    // }
    // Freeing the array of snakes
    free(state->snakes);

    // freeing the state
    free(state);
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
    for (unsigned int i = 0; i < state->num_rows; i++) {
        fprintf(fp, "%s\n", state->board[i]);
    }
}

/* Save the current state into filename. Does not modify the state object. */
void save_board(game_state_t *state, char *filename) {
    FILE *f = fopen(filename, "w");
    if (f != NULL) {
        print_board(state, f);
        fclose(f);
    }
}

/* Task 4.1 */
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) {
    return state->board[row][col];
}

static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
    state->board[row][col] = ch;
}

static bool is_tail(char c) {
    return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

static bool is_head(char c) {
    return c == 'W' || c == 'A' || c == 'S' || c == 'D';
}

static bool is_snake(char c) { 
    return is_head(c) || is_tail(c) || c == '^' || c == '<' || c == 'v';
}

static char body_to_tail(char c) {
    switch(c) {
        case '^': return 'w';
        case '<': return 'a';
        case 'v': return 's';
        default: return c;
    }
}

static char head_to_body(char c) {
    switch(c) {
        case 'W': return '^';
        case 'A': return '<';
        case 'S': return 'v';
        case 'D': return '>';
        default: return c;
    }
}

static unsigned int get_next_row(unsigned int cur_row, char c) {
    if (c == 'v' || c == 's' || c == 'S') return cur_row + 1;
    if (c == '^' || c == 'w' || c == 'W') return cur_row - 1;
    return cur_row;
}

static unsigned int get_next_col(unsigned int cur_col, char c) {
    if (c == '>' || c == 'd' || c == 'D') return cur_col + 1;
    if (c == '<' || c == 'a' || c == 'A') return cur_col - 1;
    return cur_col;
}

/* Task 4.2 */
static char next_square(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);
    unsigned int next_row = get_next_row(snake->head_row, state->board[snake->head_row][snake->head_col]);
    unsigned int next_col = get_next_col(snake->head_col, state->board[snake->head_row][snake->head_col]);
    return get_board_at(state, next_row, next_col);
}

/* Task 4.3 */
static void update_head(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);

    // Print board before update
    printf("Board before update:\n");
    print_board(state, stdout);  // Print to standard output

    unsigned int next_row = get_next_row(snake->head_row, state->board[snake->head_row][snake->head_col]);
    unsigned int next_col = get_next_col(snake->head_col, state->board[snake->head_row][snake->head_col]);

    // Store the previous head value before moving
    char prev_head_value = state->board[snake->head_row][snake->head_col];
    
    // Update the current head position to body
    set_board_at(state, snake->head_row, snake->head_col, head_to_body(prev_head_value));

    // Move the head
    snake->head_row = next_row;
    snake->head_col = next_col;

    // Update the new head position
    set_board_at(state, next_row, next_col, prev_head_value);
    
    // Print board after update
    printf("Board after update:\n");
    print_board(state, stdout);
}

/* Task 4.4 */
static void update_tail(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);

    set_board_at(state, snake->tail_row, snake->tail_col, ' ');

    unsigned int new_tail_row = get_next_row(snake->tail_row, state->board[snake->tail_row][snake->tail_col]);
    unsigned int new_tail_col = get_next_col(snake->tail_col, state->board[snake->tail_row][snake->tail_col]);

    snake->tail_row = new_tail_row;
    snake->tail_col = new_tail_col;

    set_board_at(state, new_tail_row, new_tail_col, body_to_tail(state->board[new_tail_row][new_tail_col]));
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
    for (unsigned int i = 0; i < state->num_snakes; i++) {
        char next = next_square(state, i);
        if (next == ' ' || next == '*') {
            update_head(state, i);
            if (next == ' ') update_tail(state, i); 
        }
        if (next == '*') {
            add_food(state);
        }
    }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
    char *line = (char*) malloc(LINE_SIZE * sizeof(char));
    if (line == NULL) {
        return NULL;
    }
    if (fgets(line, LINE_SIZE, fp) != NULL) {
        return line;
    }
    free(line);
    return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
    char line[LINE_SIZE];
    game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));
    if (state == NULL) {
        printf("Memory allocation for game_state_t failed.\n");
        return NULL;
    }

    state->board = (char**) malloc(NUM_ROWS * sizeof(char*));
    if (state->board == NULL) {
        printf("Memory allocation for board rows failed.\n");
        free(state);
        return NULL;
    }

    for (unsigned int i = 0; i < NUM_ROWS; i++) {
        if (fgets(line, LINE_SIZE, fp) == NULL) {
            printf("Failed to read line %u from file.\n", i);
            for (unsigned int j = 0; j < i; j++) {
                free(state->board[j]);
            }
            free(state->board);
            free(state);
            return NULL;
        }
        printf("Read line: %s", line);

        state->board[i] = (char*) malloc((NUM_COLS + 1) * sizeof(char));
        if (state->board[i] == NULL) {
            printf("Memory allocation for board row %u failed.\n", i);
            for (unsigned int j = 0; j < i; j++) {
                free(state->board[j]);
            }
            free(state->board);
            free(state);
            return NULL;
        }
        strcpy(state->board[i], line);
    }

    state->num_snakes = 1;
    state->snakes = (snake_t*) malloc(state->num_snakes * sizeof(snake_t));
    if (state->snakes == NULL) {
        printf("Memory allocation for snakes failed.\n");
        for (unsigned int i = 0; i < NUM_ROWS; i++) {
            free(state->board[i]);
        }
        free(state->board);
        free(state);
        return NULL;
    }

    state->snakes[0].tail_row = 2;
    state->snakes[0].tail_col = 2;
    state->snakes[0].head_row = 2;
    state->snakes[0].head_col = 4;
    state->snakes[0].live = true;

    return state;
}

//Task 6.1

  /* Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function
  // Starting at the snake's tail
  snake_t* snake = &(state->snakes[snum]);
  unsigned int row = snake->tail_row;
  unsigned int col = snake->tail_col;

  // Keep moving in the direction of the body until we find the head
  char c = get_board_at(state, row, col);

  while (!is_head(c)) {
  // Move to the next part of the snake based on the current body's direction
  row = get_next_row(row, c);
  col = get_next_col(col, c);
  c = get_board_at(state, row, col);
  }

  // Set the head's row and col in the snake struct
  snake->head_row = row;
  snake->head_col = col;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  if (state == NULL || state->board == NULL) {
        return NULL;
    }
  
  unsigned int snake_count = 0;
  // First, count the number of snakes by finding all tail characters
  for (unsigned int row = 0; row < state->num_rows; row++) {
      for (unsigned int col = 0; col < strlen(state->board[row]); col++) {
          if (is_tail(get_board_at(state, row, col))) {
              snake_count++;
          }
      }
  }

  // Allocate memory for all the snakes
  state->num_snakes = snake_count;
  state->snakes = (snake_t *) malloc(snake_count * sizeof(snake_t));
  if (state->snakes == NULL) {
        return NULL; // Handle allocation failure
    }

  // Now, initialize each snake by finding its tail and head
  unsigned int snum = 0;
  for (unsigned int row = 0; row < state->num_rows; row++) {
      for (unsigned int col = 0; col < strlen(state->board[row]); col++) {
          if (is_tail(get_board_at(state, row, col))) {
              // Initialize the snake with its tail position
              state->snakes[snum].tail_row = row;
              state->snakes[snum].tail_col = col;
              // state->snakes[snum].live = true;

              // Find and set the head position of the snake
              find_head(state, snum);
              snum++;
          }
      }
  }
  return state;
  
}

