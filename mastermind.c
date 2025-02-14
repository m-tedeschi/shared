#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define CODE_LENGTH 4
#define NUM_COLORS 6
#define MAX_CODES 1296  // 6^4 possible codes

const char *COLORS[] = {"r", "g", "b", "bu", "y", "w"};

typedef struct {
    int code[CODE_LENGTH];
    bool possible;
} CodeSet;

CodeSet all_codes[MAX_CODES];

// Convert color string to integer representation
int color_to_index(char *color) {
    for (int i = 0; i < NUM_COLORS; i++) {
        if (strcmp(color, COLORS[i]) == 0) {
            return i;
        }
    }
    return -1; // Error case
}

// Initialize all possible codes
void initialize_codes() {
    for (int i = 0; i < MAX_CODES; i++) {
        int value = i;
        for (int j = 0; j < CODE_LENGTH; j++) {
            all_codes[i].code[j] = value % NUM_COLORS;
            value /= NUM_COLORS;
        }
        all_codes[i].possible = true;
    }
}

// Compare two codes and return black and white peg counts
void evaluate_guess(int *guess, int *code, int *black, int *white) {
    bool guessed[CODE_LENGTH] = {false};
    bool coded[CODE_LENGTH] = {false};
    
    *black = 0;
    *white = 0;
    
    // Count black pegs
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == code[i]) {
            (*black)++;
            guessed[i] = true;
            coded[i] = true;
        }
    }
    
    // Count white pegs
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (!guessed[i]) {
            for (int j = 0; j < CODE_LENGTH; j++) {
                if (!coded[j] && guess[i] == code[j]) {
                    (*white)++;
                    coded[j] = true;
                    break;
                }
            }
        }
    }
}

// Eliminate impossible codes
void prune_codes(int *guess, int black, int white) {
    for (int i = 0; i < MAX_CODES; i++) {
        if (all_codes[i].possible) {
            int test_black, test_white;
            evaluate_guess(guess, all_codes[i].code, &test_black, &test_white);
            if (test_black != black || test_white != white) {
                all_codes[i].possible = false;
            }
        }
    }
}

// Find the next possible guess
void next_guess(int *guess) {
    for (int i = 0; i < MAX_CODES; i++) {
        if (all_codes[i].possible) {
            memcpy(guess, all_codes[i].code, CODE_LENGTH * sizeof(int));
            return;
        }
    }
}

// Mastermind solver using user-provided guesses and feedback
void mastermind_solver() {
    initialize_codes();
    char input[50];
    int num_pre_scored;
    printf("Enter the number of pre-scored guesses: ");
    scanf("%d", &num_pre_scored);
    getchar(); // Consume newline
    
    for (int i = 0; i < num_pre_scored; i++) {
        char guess_str[CODE_LENGTH][3], score_str[CODE_LENGTH][3];
        int guess[CODE_LENGTH], black = 0, white = 0;
        printf("Enter guess and score (format: g y r bu w w w w): ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s %s %s %s %s %s %s %s", 
               guess_str[0], guess_str[1], guess_str[2], guess_str[3],
               score_str[0], score_str[1], score_str[2], score_str[3]);
        
        for (int j = 0; j < CODE_LENGTH; j++) {
            guess[j] = color_to_index(guess_str[j]);
        }
        
        for (int j = 0; j < CODE_LENGTH; j++) {
            if (strcmp(score_str[j], "b") == 0) {
                black++;
            } else if (strcmp(score_str[j], "w") == 0) {
                white++;
            }
        }
        
        prune_codes(guess, black, white);
    }
    
    int guess[CODE_LENGTH];
    int attempt = 1;
    while (1) {
        next_guess(guess);
        
        int black = 0, white = 0;
        for (int i = 0; i < MAX_CODES; i++) {
            if (all_codes[i].possible) {
                evaluate_guess(guess, all_codes[i].code, &black, &white);
                if (black == CODE_LENGTH) {
                    printf("Solution found in %d attempts: ", attempt);
                    for (int j = 0; j < CODE_LENGTH; j++) {
                        printf("%s ", COLORS[guess[j]]);
                    }
                    printf("\n");
                    return;
                }
            }
        }
        prune_codes(guess, black, white);
        attempt++;
    }
}

int main() {
    printf("Solving Mastermind using pre-scored guesses...\n");
    mastermind_solver();
    return 0;
}

