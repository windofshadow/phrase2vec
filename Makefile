CC = clang

CFLAGS = -lm -pthread -Ofast -march=native -Wall -funroll-loops -Wno-unused-result
all: phrase2vec

phrase2vec : phrase2vec.c
	$(CC) phrase2vec.c SuffixTrie.c -o phrase2vec $(CFLAGS)
	
clean:
	rm -rf phrase2vec