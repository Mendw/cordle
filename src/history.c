#include "../include/history.h"

SavedGame_t *create_saved_game(time_t timestamp, char* word, Wordle_t *wordle_game) {
    int word_length = strlen(word);
    char *_word = malloc(sizeof(char) * (word_length + 1));
    strcpy(_word, word);

    SavedGame_t *saved_game = malloc(sizeof(SavedGame_t));
    saved_game->timestamp = timestamp;
    saved_game->word = _word;
    saved_game->wordle_game = wordle_game;

    saved_game->next = NULL;
    saved_game->prev = NULL;

    return saved_game;
}

void insert_saved_game(SavedGame_t **saved_games_ptr, SavedGame_t *saved_game) {
    if (*saved_games_ptr == NULL) {
        *saved_games_ptr = saved_game;
        return;
    }

    SavedGame_t *_saved_game = *saved_games_ptr;
    while (_saved_game->next != NULL) {
        _saved_game = _saved_game->next;
    }

    _saved_game->next = saved_game;
    saved_game->prev = _saved_game;
}

SavedGame_t *load_saved_games() {
    FILE *file_ptr = fopen(HISTORY_FILENAME, "a+");
    if (file_ptr == NULL) { return NULL; }

    fseek(file_ptr, 0, SEEK_SET);
    SavedGame_t * saved_games = NULL;
    SavedGame_t *last_saved_game;
    
    int max_attempts, n_attempts;
    long timestamp;

    char *word =  malloc(sizeof(char) * (WORDLE_SIZE + 1));
    char *guess = malloc(sizeof(char) * (WORDLE_SIZE + 1));

    while (!feof(file_ptr)) {
        int assigned = fscanf(file_ptr, "%s %d %d %ld\n", word, &max_attempts, &n_attempts, &timestamp);
        if (assigned < 4) break;

        Wordle_t *wordle_game = create_wordle_game(max_attempts);
        for (int attempt_idx = 0; attempt_idx < n_attempts; attempt_idx++) {
            fscanf(file_ptr, "%s\n", guess);
            LetterState_e *feedback = get_feedback(wordle_game, word, guess);
            insert_attempt(wordle_game, guess, feedback);
        }

        fscanf(file_ptr, "%*s\n");

        SavedGame_t *saved_game = create_saved_game(timestamp, word, wordle_game);
        insert_saved_game(&saved_games, saved_game);
        last_saved_game = saved_game;
    }

    free(word);
    free(guess);
    
    if (saved_games != NULL && last_saved_game != NULL) {
        last_saved_game->next = saved_games;
        saved_games->prev = last_saved_game;
    }

    fclose(file_ptr);
    return saved_games;
}

void save_game(char *word, Wordle_t *wordle_game) {
    FILE *file_ptr = fopen(HISTORY_FILENAME, "a+");
    if (file_ptr == NULL) { return; }

    time_t timestamp = time(NULL);
    fprintf(file_ptr, "%s %d %d %ld\n", word, wordle_game->max_attempts, wordle_game->attempts_made, timestamp);

    Attempt_t *attempt = wordle_game->attempts;
    while (attempt != NULL) {
        fprintf(file_ptr, "%s\n", attempt->guess);
        attempt = attempt->next;
    }

    fprintf(file_ptr, "---\n");
    fclose(file_ptr);
}

void free_saved_game(SavedGame_t *saved_game) {
    if (saved_game == NULL) return;
    saved_game->prev->next = NULL;

    free_saved_game(saved_game->next);
    free_wordle_game(saved_game->wordle_game);
    free(saved_game);
}