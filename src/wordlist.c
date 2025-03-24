#include "../include/wordlist.h"

WordList_t *create_wordlist_node(char *word) {
    int word_size = strlen(word);

    char *_word = malloc(sizeof(char) * (word_size + 1));
    strcpy(_word, word);

    WordList_t *wordlist_node = malloc(sizeof(WordList_t));
    wordlist_node->word = _word;
    wordlist_node->next = NULL;

    return wordlist_node;
}

void insert_wordlist_node(WordList_t **wordlist_ptr, WordList_t *node) {
    if (*wordlist_ptr == NULL) {
        *wordlist_ptr = node;
        return;
    }

    WordList_t *wordlist = *wordlist_ptr;
    while (wordlist->next != NULL) {
        wordlist = wordlist->next;
    }

    wordlist->next = node;
}

WordList_t *load_word_list(FILE *file) {
    char buffer[BUFFER_SIZE + 1];

    WordList_t *wordlist = NULL;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        int newline_index = strcspn(buffer, "\r\n"); 
        buffer[newline_index] = '\0';

        WordList_t *node = create_wordlist_node(buffer);

        insert_wordlist_node(&wordlist, node);
    }

    if (!feof(file)) { return NULL; }
    return wordlist;
}

int wordlist_length(WordList_t *word_list) {
    int length = 0;
    while (word_list != NULL) {
        word_list = word_list->next;
        length++;
    }

    return length;
}

char *pick_random_word(WordList_t *word_list) {
    int random_index = rand() % wordlist_length(word_list);
    
    while (random_index > 0) {
        word_list = word_list->next;
        random_index--;
    };

    return word_list->word;
}