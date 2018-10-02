
time ./phrase2vec -train text8-norm1-phrase1 -output vectors.bin -cbow 0 -size 200 -window 10 -negative 25 -hs 0 -sample 1e-5 -threads 8 -binary 1 -iter 5
./distance vectors-phrase.bin
