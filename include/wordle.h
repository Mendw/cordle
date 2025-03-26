#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>
#include <string.h>

#include "trie.h"

#define WORDLE_SIZE 5

#define INCORRECT_PAIR 1
#define MISPLACED_PAIR 2
#define CORRECT_PAIR 3

typedef enum LetterState { 
    NONE, INCORRECT, MISPLACED, CORRECT 
} LetterState_e;


typedef struct Attempt {
    char *guess;
    LetterState_e *feedback;
    struct Attempt *next;
} Attempt_t;

typedef struct Wordle {
    Attempt_t *attempts;
    char *letters;

    int max_attempts;
    int attempts_made;
} Wordle_t;

Wordle_t *create_wordle_game(int);
LetterState_e *get_feedback(Wordle_t *, char *, char *);
void insert_attempt(Wordle_t *, char *, LetterState_e *);
bool search_attempts(Attempt_t *, char *);
void free_wordle_game(Wordle_t *);

#endif