#include "../include/main.h"

void init_curses() {
    start_color();
    keypad(stdscr, TRUE);

    init_pair(INCORRECT_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MISPLACED_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CORRECT_PAIR, COLOR_BLACK, COLOR_GREEN);
}

void set_output_color(char f) {
    switch (f) {
    case INCORRECT:
        attron(COLOR_PAIR(INCORRECT));
        break;
    case MISPLACED:
        attron(COLOR_PAIR(MISPLACED));
        break;
    case CORRECT:
        attron(COLOR_PAIR(CORRECT));
        break;
    default:
        break;
    }
}

void reset_output_color(char f) {
    switch (f) {
    case INCORRECT:
        attroff(COLOR_PAIR(INCORRECT));
        break;
    case MISPLACED:
        attroff(COLOR_PAIR(MISPLACED));
        break;
    case CORRECT:
        attroff(COLOR_PAIR(CORRECT));
        break;
    default:
        break;
    }
}

void display_attempt(Attempt_t *attempt) {
    for (int index = 0; index < WORDLE_SIZE; index++) {
        LetterState_e f = attempt->feedback[index];
        
        set_output_color(f);
        addch(attempt->guess[index]);
        reset_output_color(f);
    }

    addch('\n');
}

void show_attempts(Attempt_t *attempts) {
    if (attempts == NULL) {
        addch('\n');
        return;
    }

    while(attempts != NULL) {
        display_attempt(attempts);
        attempts = attempts->next;
    }
} 

void show_letters(char *prefixes) {
    char first_prefix = 'a';
    for (int index = 0; index < PREFIX_COUNT; index++) {
        char f = prefixes[index];

        set_output_color(f);
        addch(first_prefix + index);
        reset_output_color(f);
        addch(' ');
    }

    addch('\n');
}

void show_wordle_state(Wordle_t *wordle_game) {
    show_letters (wordle_game->letters );
    printw("--- \n");

    show_attempts(wordle_game->attempts);
    printw("--- \n");
}

void show_prompt(char *guess, bool is_valid_word, bool already_guessed) {
    if (!is_valid_word) {
        printw("invalid word: %5s\n", guess);
    } else if (already_guessed) {
        printw("already guessed: %5s\n", guess);
    }

    printw("> ");
}

Wordle_t *run_wordle(char* word, Trie_t *word_list_trie, int max_attempts, bool* word_found) {
    *word_found = false;

    Wordle_t *wordle_game = create_wordle_game(max_attempts);
    char guess[WORDLE_SIZE + 1];

    curs_set(1);
    while (wordle_game->attempts_made < wordle_game->max_attempts) {
        bool first_guess = true;
        bool already_guessed = false;
        bool is_valid_word = true;

        do { 
            erase();
            printw("Cordle - '.exit' to give up\n");
            show_wordle_state(wordle_game);
            printw("Attempts: (%02d/%02d)\n", wordle_game->attempts_made, wordle_game->max_attempts);
            show_prompt(guess, is_valid_word, already_guessed);
            refresh(); getnstr(guess, WORDLE_SIZE);

            if (strcmp(guess, ".exit") == 0) return wordle_game;

            first_guess = false;
            is_valid_word = search_trie(word_list_trie, guess);
            already_guessed = search_attempts(wordle_game->attempts, guess);
        }  while (!is_valid_word || already_guessed);

        LetterState_e *feedback = get_feedback(wordle_game, word, guess);
        insert_attempt(wordle_game, guess, feedback);

        bool is_word = strcmp(word, guess) == 0;
        if (is_word) {
            *word_found = true;
            break;
        }
    }

    return wordle_game;
}

void handle_game_won(char *word, Wordle_t *wordle_game) {
    printw("Game won after %d attempts, word was %s\n", wordle_game->attempts_made, word);
}

void handle_game_lost(char *word, Wordle_t *wordle_game) {
    printw("Game lost, word was %s\n", word);
}

void run_game(WordList_t *word_list, Trie_t *word_list_trie) {
    char *word = pick_random_word(word_list);
    
    bool word_found; echo();
    Wordle_t *wordle_game = run_wordle(word, word_list_trie, MAX_ATTEMPTS, &word_found);
    
    erase();
    printw("Game over -\n");
    show_wordle_state(wordle_game);
    
    if (word_found) handle_game_won (word, wordle_game);
    else            handle_game_lost(word, wordle_game);

    printw("Save game? [y][N] > ");
    char input[2];
    getnstr(input, 1);

    switch (input[0]) {
    case 'y':
    case 'Y':
        save_game(word, wordle_game);
        break;
    
    default:
        break;
    }

    free(word);
    free_wordle_game(wordle_game);
}

void show_history_item(SavedGame_t *saved_game, bool delete_intent) {
    erase();
    Wordle_t *wordle_game = saved_game->wordle_game;

    char timestring_buffer[32];
    struct tm *timeinfo = localtime(&saved_game->timestamp);
    strftime(timestring_buffer, 32, "%d/%m/%Y - %I:%M %p", timeinfo);

    printw("<- %s->\n", timestring_buffer);
    printw("  [%2d/%2d] Attempts\n", wordle_game->attempts_made, wordle_game->max_attempts);
    printw("  [%s]\n\n", saved_game->word);
    
    show_wordle_state(wordle_game);

    for (int line_index = wordle_game->max_attempts - wordle_game->attempts_made; line_index > 0; line_index--) addch('\n');

    if (delete_intent) {
        printw("\narrows to move, 'q' to quit, 'x' again to confirm\n");
    } else {
        printw("\narrows to move, 'q' to quit, 'x' to delete\n");
    }
    refresh();
}

void show_history() {
    SavedGame_t *saved_games = load_saved_games();
    if (saved_games == NULL) {
        printw("\nUnable to open history file");
        getch(); return;
    }
    
    SavedGame_t *selected_game = saved_games;
    bool exit = false;
    bool delete_intent = false;
    while (!exit && selected_game != NULL) {
        show_history_item(selected_game, delete_intent);
        switch (getch()) {
            case KEY_LEFT:
            case 'a': case 'A':
                selected_game = selected_game->prev;
                delete_intent = false;
                break;
            case KEY_RIGHT:
            case 'd': case 'D':
                selected_game = selected_game->next;
                delete_intent = false;
                break;
            case 'x': case 'X':
                if (delete_intent) {
                    delete_saved_game(&saved_games, &selected_game);
                    delete_intent = false;
                    break;
                }

                delete_intent = true;
                break;
            case 'q': case 'Q':
                exit = true;
                break;
        }
    }

    free_saved_game(selected_game);
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

bool select_menu_option(char selected, WordList_t *word_list, Trie_t *word_list_trie) {
    switch (selected) {
        case PLAY_GAME:
            run_game(word_list, word_list_trie);
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

void show_menu(WordList_t *word_list, Trie_t *word_list_trie) {
    curs_set(0);
    noecho();
    
    char n_options = 3; 
    char selected = PLAY_GAME;
    
    while (true) {
        print_menu(selected);
        switch (getch()) {
            case KEY_UP:
            case 'w': case 'W':
                selected += n_options - 1;
                selected %= n_options;
                break;
            case KEY_DOWN:
            case 's': case 'S':
                selected += 1;
                selected %= n_options;
                break;
            case '\n': case KEY_ENTER:
            case KEY_RIGHT: 
            case 'd': case 'D':
                bool should_quit = select_menu_option(selected, word_list, word_list_trie);
                if (should_quit) return; 
                
                curs_set(0);
                noecho();
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    WordList_t *word_list = load_word_list();
    if (word_list == NULL) {
        printf("Unable to parse words file");
        return 1;
    }
    
    initscr();
    init_curses();
    
    Trie_t *word_list_trie = generate_trie(word_list);
    show_menu(word_list, word_list_trie);
    endwin();
    return 0;
}