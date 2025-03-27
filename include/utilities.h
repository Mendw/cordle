#ifndef UTILITIES_H
#define UTILITIES_H

#include <ncurses.h>
#include "wordle.h"

void init_curses();
void set_output_color(LetterState_e);
void reset_output_color(LetterState_e);
void getscrtnstr(char *, int);

#endif