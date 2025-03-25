#include "../include/wordle.h"

enum LetterState { NONE, INCORRECT, MISPLACED, CORRECT };

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

Wordle_t *init_wordle(int max_attempts) {
    Wordle_t *wordle_game = malloc(sizeof(Wordle_t));
    wordle_game->attempts = NULL;
    wordle_game->max_attempts = max_attempts;
    wordle_game->attempts_made = 0;
    
    wordle_game->letters  = malloc(sizeof(char) * PREFIX_COUNT);
    for (int index = 0; index < PREFIX_COUNT; index++) {
        wordle_game->letters[index] = NONE;
    }

    return wordle_game;
}

Attempt_t *create_attempt_node(char *guess, char *feedback) {
    int guess_size = strlen(guess);
    char *_guess = malloc(sizeof(char) * (guess_size + 1));
    strcpy(_guess, guess);
    
    Attempt_t * node = malloc(sizeof(Attempt_t)); 
    node->guess = _guess;
    node->feedback = feedback;
    node->next = NULL;

    return node;
}

void insert_attempt(Wordle_t *wordle, char *word, char *guess) {
    char *feedback = malloc(sizeof(char) * (WORDLE_SIZE + 1));
    for (int index = 0; index < WORDLE_SIZE; index++) {
        int prefix_offset = get_prefix_offset(guess[index]);
        if (word[index] == guess[index]) {
            wordle->letters[prefix_offset] = CORRECT;
            feedback[index] = CORRECT;
            continue;
        }

        if (strchr(word, guess[index]) != NULL) {
            if (wordle->letters[prefix_offset] != CORRECT) {
                wordle->letters[prefix_offset] = MISPLACED;
            }

            feedback[index] = MISPLACED;
            continue;
        }

        wordle->letters[prefix_offset] = INCORRECT;
        feedback[index] = INCORRECT;
    }

    Attempt_t *node = create_attempt_node(guess, feedback);
    wordle->attempts_made += 1;
    if (wordle->attempts == NULL) {
        wordle->attempts = node;
        return;
    }

    Attempt_t *attempt = wordle->attempts;
    while (attempt->next != NULL) attempt = attempt->next;
    attempt->next = node;
}

bool search_attempts(Attempt_t *attempt, char *guess) {
    while (attempt != NULL) {
        if (strcmp(attempt->guess, guess) == 0) {
            return true;
        }

        attempt = attempt->next;
    }

    return false;
}

void display_attempt(Attempt_t *attempt) {
    for (int index = 0; index < WORDLE_SIZE; index++) {
        char f = attempt->feedback[index];
        
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

void show_wordle_state(Wordle_t *wordle) {
    show_letters (wordle->letters );
    printw("--- \n");

    show_attempts(wordle->attempts);
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

    Wordle_t *wordle = init_wordle(max_attempts);
    char guess[WORDLE_SIZE + 1];

    curs_set(1);
    while (wordle->attempts_made < wordle->max_attempts) {
        bool first_guess = true;
        bool already_guessed = false;
        bool is_valid_word = true;

        do { 
            erase();
            printw("Cordle - '.exit' to give up\n");
            show_wordle_state(wordle);
            printw("Attempts: (%02d/%02d)\n", wordle->attempts_made, wordle->max_attempts);
            show_prompt(guess, is_valid_word, already_guessed);
            refresh(); getnstr(guess, WORDLE_SIZE);

            if (strcmp(guess, ".exit") == 0) return wordle;

            first_guess = false;
            is_valid_word = search_trie(word_list_trie, guess);
            already_guessed = search_attempts(wordle->attempts, guess);
        }  while (!is_valid_word || already_guessed);

        insert_attempt(wordle, word, guess);
        bool is_word = strcmp(word, guess) == 0;
        if (is_word) {
            *word_found = true;
            break;
        }
    }

    return wordle;
}