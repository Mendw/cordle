#ifndef WORDLIST_T
#define WORDLIST_T

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 10
#define WORDLIST_FILENAME "words.txt"

typedef struct Wordlist {
    char *word;
    struct Wordlist *next;
} WordList_t;

WordList_t *load_word_list();
char *pick_random_word(WordList_t *);

#endif