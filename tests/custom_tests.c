#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "state.h"
#include "snake_utils.h"

/* Helper function prototypes */
bool test_is_tail();
bool test_is_head();
bool test_is_snake();
bool test_body_to_tail();
bool test_head_to_body();
bool test_get_next_row();
bool test_get_next_col();
bool test_next_square();
bool test_update_head();
bool test_update_tail();

/* Main function to run all tests */
int main() {
    if (test_is_tail() &&
        test_is_head() &&
        test_is_snake() &&
        test_body_to_tail() &&
        test_head_to_body() &&
        test_get_next_row() &&
        test_get_next_col() &&
        test_next_square() &&
        test_update_head() &&
        test_update_tail()) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("Some tests failed.\n");
        return 1;
    }
}

/* Test helper functions */
bool test_is_tail() {
    return is_tail('w') && is_tail('a') && is_tail('s') && is_tail('d') &&
           !is_tail('W') && !is_tail('X') && !is_tail(' ') && !is_tail('*');
}

bool test_is_head() {
    return is_head('W') && is_head('A') && is_head('S') && is_head('D') &&
           !is_head('w') && !is_head('x') && !is_head(' ') && !is_head('*');
}

bool test_is_snake() {
    return is_snake('w') && is_snake('a') && is_snake('s') && is_snake('d') &&
           is_snake('W') && is_snake('A') && is_snake('S') && is_snake('D') &&
           is_snake('^') && is_snake('<') && is_snake('v') && is_snake('>') &&
           !is_snake(' ') && !is_snake('*') && !is_snake('X');
}

bool test_body_to_tail() {
    return body_to_tail('^') == 'w' && body_to_tail('<') == 'a' &&
           body_to_tail('v') == 's' && body_to_tail('>') == 'd' &&
           body_to_tail('w') == 'w' && body_to_tail('a') == 'a' &&
           body_to_tail('s') == 's' && body_to_tail('d') == 'd';
}

bool test_head_to_body() {
    return head_to_body('W') == '^' && head_to_body('A') == '<' &&
           head_to_body('S') == 'v' && head_to_body('D') == '>' &&
           head_to_body('w') == 'w' && head_to_body('a') == 'a' &&
           head_to_body('s') == 's' && head_to_body('d') == 'd';
}

bool test_get_next_row() {
    return get_next_row(5, 'v') == 6 && get_next_row(5, 's') == 6 &&
           get_next_row(5, 'S') == 6 && get_next_row(5, '^') == 4 &&
           get_next_row(5, 'w') == 4 && get_next_row(5, 'W') == 4 &&
           get_next_row(5, '>') == 5 && get_next_row(5, '<') == 5;
}

bool test_get_next_col() {
    return get_next_col(5, '>') == 6 && get_next_col(5, 'd') == 6 &&
           get_next_col(5, 'D') == 6 && get_next_col(5, '<') == 4 &&
           get_next_col(5, 'a') == 4 && get_next_col(5, 'A') == 4 &&
           get_next_col(5, 'v') == 5 && get_next_col(5, '^') == 5;
}

bool test_next_square() {
    game_state_t state;
    state.num_rows = 3;
    state.num_snakes = 1;
    state.snakes = malloc(sizeof(snake_t));
    state.snakes[0].head_row = 1;
    state.snakes[0].head_col = 1;
    state.board = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        state.board[i] = malloc(4 * sizeof(char));
        strcpy(state.board[i], "   ");
    }
    state.board[1][1] = '>';
    state.board[1][2] = '^';
    state.board[2][1] = '*';
    
    bool result = next_square(&state, 0) == '*';

    for (int i = 0; i < 3; i++) {
        free(state.board[i]);
    }
    free(state.board);
    free(state.snakes);

    return result;
}

bool test_update_head() {
    // Create a game state with a snake
    game_state_t state;
    state.num_rows = 3;
    state.num_snakes = 1;
    state.snakes = malloc(sizeof(snake_t));
    state.snakes[0].head_row = 1;
    state.snakes[0].head_col = 1;
    state.snakes[0].tail_row = 1;
    state.snakes[0].tail_col = 0;
    state.snakes[0].live = true;
    state.board = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        state.board[i] = malloc(4 * sizeof(char));
        strcpy(state.board[i], "   ");
    }
    state.board[1][1] = '>';
    state.board[1][0] = 'w';
    state.board[2][1] = '*';
    
    update_head(&state, 0);
    
    bool result = state.board[1][1] == ' ' && state.board[2][1] == '>' &&
                   state.snakes[0].head_row == 2 && state.snakes[0].head_col == 1;

    for (int i = 0; i < 3; i++) {
        free(state.board[i]);
    }
    free(state.board);
    free(state.snakes);

    return result;
}

bool test_update_tail() {
    // Create a game state with a snake
    game_state_t state;
    state.num_rows = 3;
    state.num_snakes = 1;
    state.snakes = malloc(sizeof(snake_t));
    state.snakes[0].tail_row = 1;
    state.snakes[0].tail_col = 0;
    state.snakes[0].head_row = 1;
    state.snakes[0].head_col = 1;
    state.snakes[0].live = true;
    state.board = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        state.board[i] = malloc(4 * sizeof(char));
        strcpy(state.board[i], "   ");
    }
    state.board[1][0] = 'w';
    state.board[1][1] = '>';
    state.board[2][0] = '*';
    
    update_tail(&state, 0);
    
    bool result = state.board[1][0] == ' ' && state.board[2][0] == 'w' &&
                   state.snakes[0].tail_row == 2 && state.snakes[0].tail_col == 0;

    for (int i = 0; i < 3; i++) {
        free(state.board[i]);
    }
    free(state.board);
    free(state.snakes);

    return result;
}
