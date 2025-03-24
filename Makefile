all: build run

build: src/main.c src/wordlist.c src/trie.c src/wordle.c
	@mkdir -p build
	gcc src/main.c src/wordlist.c src/trie.c src/wordle.c -o build/wordle.out -lncurses

run:
	@./build/wordle.out words.txt