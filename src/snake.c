#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

// Task 1: Creating the default_state function 

game_state_t* create_default_state(){
    // Allocating memory for the game state
    game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));

    // Creating the board
    state->num_of_rows = 18;
    state->board = (char**) malloc(state->num_of_rows * sizeof(char*));

    for (int i = 0; i < stat->num_of_rows; i++){
        state->board[i] = (char*) malloc(21 * sizeof(char));
        strcpy(state->board[i], "####################");
    }

    strcpy(state->board[2], "# d>D     *            #");

    state->num_snakes = 1;
    state->snakes = (snake_t*) malloc(sizeof(snake_t));

    //setting the snake's head and tail postions 
    state->snakes[0].tail_row = 2;
    state->snakes[0].tail_col = 2;
    state->snakes[0].head_row = 2;
    state->snakes[0].head_col = 4;
    state->snakes[0].live = true;

    return state;

}

int main(int argc, char *argv[]) {
  bool io_stdin = false;
  char *in_filename = NULL;
  char *out_filename = NULL;
  game_state_t *state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    // TODO: If the file doesn't exist, return -1
    // TODO: Then call initialize_snakes on the state you made
    // TODO: close file pointer
  } else if (io_stdin) {
    // TODO: Load the board from stdin
    // TODO: Then call initialize_snakes on the state you made
  } else {
    // TODO: Create default state
  }

  // TODO: Update state. Use the deterministic_food function
  // (already implemented in snake_utils.h) to add food.

  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
  } else {
    // TODO: Print the board to stdout
  }

  // TODO: Free the state

  return 0;
}
