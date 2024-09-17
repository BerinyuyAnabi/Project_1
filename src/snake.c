#include <stdio.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"
#define _GNU_SOURCE

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
//     if (in_filename != NULL) {
//         FILE *in_file = fopen(in_filename, "r");
//         if (in_file == NULL) {
//             fprintf(stderr, "Error: Failed to open input file '%s'.\n", in_filename);
//             return 1;
//         }
//         state = load_board(in_file);
//         fclose(in_file);
//         if (state == NULL) {
//             fprintf(stderr, "Error: Failed to load board from file '%s'.\n", in_filename);
//             return 1;
//         }
//     } else if (io_stdin) {
//         state = load_board(stdin);
//         if (state == NULL) {
//             fprintf(stderr, "Error: Failed to load board from stdin.\n");
//             return 1;
//         }
//     } else {
//         state = create_default_state();
//         if (state == NULL) {
//             fprintf(stderr, "Error: Failed to create default state.\n");
//             return 1;
//         }
//     }

//     // Initialize snakes
//     initialize_snakes(state);

//     // Update state. Use the deterministic_food function to add food.
//     deterministic_food(state);

//     // Write updated board to file or stdout
//     if (out_filename != NULL) {
//         FILE *out_file = fopen(out_filename, "w");
//         if (out_file == NULL) {
//             fprintf(stderr, "Error: Failed to open output file '%s'.\n", out_filename);
//             free_state(state);
//             return 1;
//         }
//         print_board(state, out_file);
//         fclose(out_file);
//     } else {
//         print_board(state, stdout);
//     }

//     // Free the state
//     free_state(state);
//     return 0;
// }
