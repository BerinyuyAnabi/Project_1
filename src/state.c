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

/* Task 1 */
game_state_t *create_default_state() {
  // TODO: Implement this function.
  // Allocating memory for game state
  game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));

  // Initializing the board's rows and columns
  state->num_rows = 18;
  state->board = (char**) malloc(state->num_rows * sizeof(char*));

  for(int i = 0; i< state->num_rows; i++){
      state->board[i] = (char*) malloc(21 * sizeof(char));

      // Setting borders for the walls and spaces 
      strcpy(state->board[i], "####################");
  }
  // Placing the snake and fruit on the board 
  strcpy(state->board[2], "# d>D    *          #");

  // Initialising the first snake as an array 
  state->num_snakes = 1;
  state->snakes = (snake_t*) malloc(sizeof(snake_t));

  // Creating the tail and head positions 
  state->snakes[0].tail_row = 2;
  state->snakes[0].tail_col = 2;
  state->snakes[0].head_row = 2;
  state->snakes[0].head_col = 4;
  state->snakes[0].live = true;

  return state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.
  // Freeing the board rows 
  for(int i=0; i<state->num_rows;i++){
      free(state->board[i]);
  }
  // freeing the board
  free(state->board);
  // freeing the snakes array
  free(state->snakes);

  //freeing the game_state_struct
  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  // TODO: Implement this function.
      for(int i = 0; i <state-> num_rows; i++){
          fprintf(fp, "%s\n", state->board[i]);
    }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if (c == 'w'){
      return true;
  }else if( c =='a'){
      return true;
    } else if (c == 's'){
        return true;
    } else if (c == 'd'){
        return true;
    } else{
        return false;
    }
}


/*
Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if(c == 'W'){
      return true;
  } else if (c == 'A'){
      return true;
  } else if (c == 'S'){
      return true;
  } else if (c == 'D'){
      return true;
    }else {
        return false;
    }
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) { 
  // TODO: Implement this function.
  return (is_head(c) || is_tail(c) || c == '^' || c == '<' || c == 'v');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch(c){
      case '^': return 'w';
      case '<': return 'a';
      case 'v': return 's';
      default: return c;
}
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  switch(c){
      case 'W': return '^';
      case 'A': return '<';
      case 'S': return 'v';
      case 'D': return '>';
      default: return c;
   }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if(c == 'v' || c == 's' || c == 'S')
      return cur_row + 1;
  else if(c == '^' || c == 'w' || c == 'W')
      return cur_row - 1;
  else 
      return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
    if (c == '>' || c == 'd' || c == 'D')
        return cur_col + 1;
    else if (c == '<' || c == 'a' || c == 'A')
        return cur_col - 1;
    else
        return cur_col;
}
/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);
    unsigned int next_row = get_next_row(snake->head_row, state->board[snake->head_row][snake->head_col]);
    unsigned int next_col = get_next_col(snake->head_col, state->board[snake->head_row][snake->head_col]);
    return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);
    unsigned int next_row = get_next_row(snake->head_row, state->board[snake->head_row][snake->head_col]);
    unsigned int next_col = get_next_col(snake->head_col, state->board[snake->head_row][snake->head_col]);

    // Updating the current head position to body
    set_board_at(state, snake->head_row, snake->head_col, head_to_body(state->board[snake->head_row][snake->head_col]));

    // Moving the head to the new position
    snake->head_row = next_row;
    snake->head_col = next_col;

    // Placing the new head on the board
    set_board_at(state, next_row, next_col, state->board[snake->head_row][snake->head_col]);
}


/*
  Task 4.4

  Helper function for update_state. Update the taicurl...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
    snake_t* snake = &(state->snakes[snum]);

    // Removing the current tail
    set_board_at(state, snake->tail_row, snake->tail_col, ' ');

    // Moving the tail forward
    unsigned int new_tail_row = get_next_row(snake->tail_row, state->board[snake->tail_row][snake->tail_col]);
    unsigned int new_tail_col = get_next_col(snake->tail_col, state->board[snake->tail_row][snake->tail_col]);

    snake->tail_row = new_tail_row;
    snake->tail_col = new_tail_col;

    // Updating the new tail on the board
    set_board_at(state, new_tail_row, new_tail_col, body_to_tail(state->board[new_tail_row][new_tail_col]));
}


/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_snakes; i++) {
        // Update the head of the snake
        char next = next_square(state, i);
        
        if (next == ' ' || next == '*') {
            update_head(state, i);
            if (next == ' ') update_tail(state, i); 
        }
        
        // Handle food
        if (next == '*') {
            // Place new food
            add_food(state);
        }
    }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  char *line = NULL;
    size_t len = 0;
    // ssize_t read;
    if ((getline(&line, &len, fp) != -1) {
        return line;
    }
    free(line);
    return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));

    // Reading the board line by line
    char *line;
    unsigned int row = 0;
    while ((line = read_line(fp)) != NULL) {
        strcpy(state->board[row], line);
        row++;
        free(line);
    }
    return state;
  // return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
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
  return NULL;
}
