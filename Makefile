CC = clang

CFLAGS = -lm -pthread -Ofast -march=native -Wall -funroll-loops -Wno-unused-result
all: word2vec

word2vec : word2vec.c
	$(CC) word2vec.c SuffixTrie.c -o word2vec $(CFLAGS)
	
clean:
	rm -rf word2vec