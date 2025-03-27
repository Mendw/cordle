#include "../include/utilities.h"

void init_curses() {
    start_color();
    keypad(stdscr, TRUE);

    init_pair(INCORRECT_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MISPLACED_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CORRECT_PAIR, COLOR_BLACK, COLOR_GREEN);
}

void set_output_color(LetterState_e f) {
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

void reset_output_color(LetterState_e f) {
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

void getscrtnstr(char *s, int n) {
    int index = 0;
    int character;

    noecho();
    while (true) {
        character = getch();
        if (character == '\n') break;

        bool is_backspace = character == KEY_BACKSPACE || character == 127;
        if (is_backspace) {
            if (index == 0) continue;

            // Handle backspace
            mvaddch(getcury(stdscr), getcurx(stdscr) - 1, ' ') ;
            move(getcury(stdscr), getcurx(stdscr) - 1); 
            
            index--;
            continue;
        }

        if (index == n) continue;
        s[index++] = character;
        addch('*');
    }

    s[index] = '\0';
    echo();
}