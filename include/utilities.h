#ifndef UTILITIES_H
#define UTILITIES_H

#include <ncurses.h>
#include "wordle.h"

void init_curses();
void set_output_color(char);
void reset_output_color(char);
void getscrtnstr(char *, int);

#endif