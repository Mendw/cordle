#ifndef TRIE_H
#define TRIE_H

#include "wordlist.h"
#define PREFIX_COUNT 26

typedef struct Trie {
    struct Trie **children;
    bool isleaf;
} Trie_t;

Trie_t *generate_trie(WordList_t *);
bool search_trie(Trie_t *, char *);
int get_prefix_offset(char);

#endif