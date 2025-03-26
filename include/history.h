#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <time.h>

#include "wordle.h"

#define HISTORY_FILENAME "history.txt"

typedef struct SavedGame {
    time_t timestamp;
    char *word;
    Wordle_t *wordle_game;

    struct SavedGame *prev;
    struct SavedGame *next;
} SavedGame_t;

SavedGame_t *load_saved_games();
void save_game(char *, Wordle_t *);
void free_saved_game(SavedGame_t *saved_game);
void delete_saved_game(SavedGame_t **, SavedGame_t **);

#endif