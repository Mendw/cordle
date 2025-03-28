#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#include "wordlist.h"
#include "trie.h"
#include "wordle.h"
#include "history.h"
#include "utilities.h"

#define MAX_ATTEMPTS 10
#define MAX(A, B) (((A) > (B)) ? (A) : (B))