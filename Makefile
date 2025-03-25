all: run clean

build/wordle.out: src/main.c src/wordlist.c src/trie.c src/wordle.c
	@mkdir -p build
	gcc src/main.c src/wordlist.c src/trie.c src/wordle.c -o build/wordle.out -lncurses

run: build/wordle.out
	@./build/wordle.out words.txt

clean:
	@find build/ -name '*.out' -delete