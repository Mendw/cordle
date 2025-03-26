#include "../include/wordle.h"

Wordle_t *create_wordle_game(int max_attempts) {
    Wordle_t *wordle_game = malloc(sizeof(Wordle_t));
    wordle_game->attempts = NULL;
    wordle_game->max_attempts = max_attempts;
    wordle_game->attempts_made = 0;
    
    wordle_game->letters  = malloc(sizeof(char) * PREFIX_COUNT);
    for (int index = 0; index < PREFIX_COUNT; index++) {
        wordle_game->letters[index] = NONE;
    }

    return wordle_game;
}

Attempt_t *create_attempt_node(char *guess, LetterState_e *feedback) {
    int guess_size = strlen(guess);
    char *_guess = malloc(sizeof(char) * (guess_size + 1));
    strcpy(_guess, guess);
    
    Attempt_t * node = malloc(sizeof(Attempt_t)); 
    node->guess = _guess;
    node->feedback = feedback;
    node->next = NULL;

    return node;
}

LetterState_e* get_feedback(Wordle_t *wordle_game, char *word, char *guess) {
    LetterState_e *feedback = malloc(sizeof(char) * (WORDLE_SIZE + 1));

    for (int index = 0; index < WORDLE_SIZE; index++) {
        int prefix_offset = get_prefix_offset(guess[index]);
        if (word[index] == guess[index]) {
            wordle_game->letters[prefix_offset] = CORRECT;
            feedback[index] = CORRECT;
            continue;
        }

        if (strchr(word, guess[index]) != NULL) {
            if (wordle_game->letters[prefix_offset] != CORRECT) {
                wordle_game->letters[prefix_offset] = MISPLACED;
            }

            feedback[index] = MISPLACED;
            continue;
        }

        wordle_game->letters[prefix_offset] = INCORRECT;
        feedback[index] = INCORRECT;
    }

    return feedback;
}

void insert_attempt(Wordle_t *wordle_game, char *guess, LetterState_e *feedback) {
    Attempt_t *node = create_attempt_node(guess, feedback);
    wordle_game->attempts_made += 1;
    if (wordle_game->attempts == NULL) {
        wordle_game->attempts = node;
        return;
    }

    Attempt_t *attempt = wordle_game->attempts;
    while (attempt->next != NULL) attempt = attempt->next;
    attempt->next = node;
}

bool search_attempts(Attempt_t *attempt, char *guess) {
    while (attempt != NULL) {
        if (strcmp(attempt->guess, guess) == 0) {
            return true;
        }

        attempt = attempt->next;
    }

    return false;
}

void free_wordle_attempts(Attempt_t *attempt) {
    if (attempt == NULL) return;

    free_wordle_attempts(attempt->next);
    free(attempt->guess);
    free(attempt->feedback);
    free(attempt);
}

void free_wordle_game(Wordle_t *wordle_game) {
    free(wordle_game->letters);
    free_wordle_attempts(wordle_game->attempts);
    free(wordle_game);
}