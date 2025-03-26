#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#include "wordlist.h"
#include "trie.h"
#include "wordle.h"
#include "history.h"

#define MAX_ATTEMPTS 10