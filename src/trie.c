#include "../include/trie.h"

Trie_t *create_trie_node() {
    Trie_t *node = malloc(sizeof(Trie_t));
    node->children = malloc(sizeof(Trie_t *) * PREFIX_COUNT);

    // Sets the whole array of pointers to 0
    memset(node->children, 0, sizeof(Trie_t*) * PREFIX_COUNT);
    node->isleaf = false;

    return node;
}

int get_prefix_offset(char prefix) {
    // Turns uppercase letters into lowercase letters
    if (prefix < 'a') prefix += 'a' - 'A';

    return prefix - 'a';
}

void insert_trie_node(Trie_t *trie_node, char *word) {
    for (int char_index = 0; word[char_index] != '\0'; char_index++) {
        int prefix_offset = get_prefix_offset(word[char_index]);
        if (prefix_offset < 0 || prefix_offset >= PREFIX_COUNT) return;

        if (trie_node->children[prefix_offset] == NULL) {
            Trie_t *new_node = create_trie_node();
            trie_node->children[prefix_offset] = new_node;
        }

        trie_node = trie_node->children[prefix_offset];
    }

    trie_node->isleaf = true;
}

bool search_trie(Trie_t *trie_node, char *word) {
    for (int char_index = 0; word[char_index] != '\0'; char_index++) {
        int prefix_offset = get_prefix_offset(word[char_index]);

        bool valid_prefix = prefix_offset >= 0 && prefix_offset < PREFIX_COUNT;
        if (!valid_prefix || trie_node->children[prefix_offset] == NULL) { 
            return false;
        }

        trie_node = trie_node->children[prefix_offset];
    }

    return trie_node->isleaf;
}

Trie_t *generate_trie(WordList_t *word_list) {
    Trie_t * trie_root = create_trie_node();
    while (word_list != NULL) {
        insert_trie_node(trie_root, word_list->word);
        word_list = word_list->next;
    }

    return trie_root;
}