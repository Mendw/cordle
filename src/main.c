#include "../include/main.h"

void init_curses() {
    start_color();
    keypad(stdscr, TRUE);

    init_pair(INCORRECT_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MISPLACED_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CORRECT_PAIR, COLOR_BLACK, COLOR_GREEN);
}

void handle_game_won(char *word, Wordle_t *wordle) {
    printw("Game won after %d attempts, word was %s\n", wordle->attempts_made, word);
}

void handle_game_lost(char *word, Wordle_t *wordle) {
    printw("Game lost, word was %s\n", word);
}

void run_game(WordList_t *word_list) {
    char *word = pick_random_word(word_list);
    Trie_t *word_list_trie = generate_trie(word_list);
    
    bool word_found; echo();
    Wordle_t *wordle = run_wordle(word, word_list_trie, MAX_ATTEMPTS, &word_found);
    
    erase();
    printw("Game over -\n");
    show_wordle_state(wordle);
    
    if (word_found) handle_game_won (word, wordle);
    else            handle_game_lost(word, wordle);

    printw("Press any key to continue");
    
    getch();
}

void show_history() {
    erase();
    printw("Not implemented...");

    refresh(); getch();
}

enum MenuOption { PLAY_GAME, GAME_HISTORY, QUIT_GAME };

void print_menu(char selected) {
    erase();

    printw("| --------------------- |\n");
    printw("|         MENU          |\n");
    printw("| [%c] Play game         |\n", selected == PLAY_GAME ? '>' : ' ');
    printw("| [%c] Game history      |\n", selected == GAME_HISTORY ? '>' : ' ');
    printw("|                       |\n");
    printw("| [%c] Quit              |\n", selected == QUIT_GAME ? '>' : ' ');
    printw("| --------------------- |\n");

    refresh();
}

bool select_menu_option(char selected, WordList_t *word_list) {
    switch (selected) {
        case PLAY_GAME:
            run_game(word_list);
            break;
        case GAME_HISTORY:
            show_history();
            break;
        case QUIT_GAME:
            return true;
        default:
            break;
    }

    return false;
}

void show_menu(WordList_t *word_list) {
    curs_set(0);
    noecho();
    
    char n_options = 3; 
    char selected = PLAY_GAME;
    
    while (true) {
        print_menu(selected);
        switch (getch()) {
            case KEY_UP:
            case 'w':
            case 'W':
                selected += n_options - 1;
                selected %= n_options;
                break;
            case KEY_DOWN:
            case 's':
            case 'S':
                selected += 1;
                selected %= n_options;
                break;
            case '\n': case KEY_ENTER:
            case KEY_RIGHT:
                bool should_quit = select_menu_option(selected, word_list);
                if (should_quit) return; 
                
                break;
            default:
                break;
        }
    }
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
    
    srand(time(NULL));

    initscr();
    init_curses();

    show_menu(word_list);

    endwin();
    return 0;
}