//
// Created by windofshadow on 02/06/2017.
//

#ifndef COMPOSITIONAL_SUFFIXTRIE_H
#define COMPOSITIONAL_SUFFIXTRIE_H

#endif //COMPOSITIONAL_SUFFIXTRIE_H


#include "base.h"

typedef struct Lnod{
    int id;
    struct Lnod* next;
}Lnode;

typedef struct Nod {
    char* word;
    int ref;
    struct Lnod* unref;//the list of unrefed component
    int* child_hash;
    struct Nod** child_list;
    int child_size;
    int max_size;
}Node;

void add2Trie(Node* root,int id);

Node* initTrie();
