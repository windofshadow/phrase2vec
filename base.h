//
// Created by windofshadow on 02/06/2017.
//

#ifndef COMPOSITIONAL_BASE_H
#define COMPOSITIONAL_BASE_H

#endif //COMPOSITIONAL_BASE_H

typedef struct vocab_word {
    long long cn;// I guess the cn means count of numberƒ
    int *point;
    char *word, *code, codelen;
    int wc;//word number if it is a phrase
    char** words;
    ///component id
    int* component;
    int comp_len;
    int* component_new;
    int comp_len_new;
}WORD;

struct vocab_word *vocab;