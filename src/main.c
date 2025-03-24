#include "../include/main.h"

void init_curses() {
    start_color();

    init_pair(INCORRECT_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MISPLACED_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CORRECT_PAIR, COLOR_BLACK, COLOR_GREEN);
}

void handle_game_won(char *word, Wordle_t *wordle) {
    printw("Game won after %d attempts, word was %s\n", wordle->attempts_made, word);
    refresh();
}

void handle_game_lost(char *word, Wordle_t *wordle) {
    printw("Game lost, word was %s\n", word);
    refresh();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    
    FILE *file_ptr = fopen(argv[1], "r");
    if (file_ptr == NULL) {
        printf("Unable to open file %s", argv[1]);
        return 2;
    }
    
    WordList_t *word_list = load_word_list(file_ptr);
    if (word_list == NULL) {
        printf("Unable to parse file");
        return 3;
    }
    
    initscr();
    init_curses();
    srand(time(NULL));

    char *word = pick_random_word(word_list);
    Trie_t *word_list_trie = generate_trie(word_list);
    
    bool word_found;
    Wordle_t *wordle = run_wordle(word, word_list_trie, MAX_ATTEMPTS, &word_found);
    
    erase();
    show_wordle_state(wordle);
    
    if (word_found) handle_game_won (word, wordle);
    else            handle_game_lost(word, wordle);

    printw("Press any key to exit");
    
    getch();
    endwin();
    return 0;
}