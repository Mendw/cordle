#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>
#include <ncurses.h>
#include <string.h>
#include "trie.h"

#define WORDLE_SIZE 5

#define INCORRECT_PAIR 1
#define MISPLACED_PAIR 2
#define CORRECT_PAIR 3

typedef struct Attempt {
    char *guess;
    char *feedback;

    struct Attempt *next;
} Attempt_t;

typedef struct Wordle {
    Attempt_t *attempts;
    char *letters;

    int max_attempts;
    int attempts_made;
} Wordle_t;

Wordle_t *run_wordle(char*, Trie_t*, int, bool*);
void show_wordle_state(Wordle_t *);

#endif