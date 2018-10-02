//
// Created by windofshadow on 02/06/2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SuffixTrie.h"
//#include "base.h"

#define MAX_STRING 100
#define MAX_CHILD 1000

const int child_first_layer = 30000000;//initial size of hashtable
const int child_hash_size=30000;

// Returns hash value of a word
int GetWordHash_head(char *word){
    int a;
    unsigned long long hash = 0;
    for (a = 0; a < strlen(word); a++) hash = hash * 257 + word[a];
    hash = hash % child_first_layer;
    return hash;
}

int GetWordHash_other(char *word){
    unsigned long long a, hash = 0;
    for (a = 0; a < strlen(word); a++) hash = hash * 257 + word[a];
    hash = hash % child_hash_size;
    return hash;
}

Node* SearchChild(char *word,Node* p) {
    unsigned int hash;
    if(p->child_list==NULL)
        return NULL;
    if(p->word==NULL)
        hash= GetWordHash_head(word);
    else
        hash=GetWordHash_other(word);
    while (1) {
        if (p->child_hash[hash] == -1) return NULL;
        int pos=p->child_hash[hash];
        Node* child=p->child_list[pos];
        if (!strcmp(word, child->word))
            return child;
        if(p->word==NULL)
            hash = (hash + 1) % child_first_layer;
        else
            hash=(hash+1)%child_hash_size;
    }
    return -1;
}

void add2List(Lnode* list,int id) {
    Lnode *p = list;
    if (p == NULL) {
        p = (Lnode*)malloc(sizeof(Lnode));
        p->id=id;
        p->next=NULL;
    }
    else {
        while (1) {
            if (p->next == NULL)
                break;
            p = p->next;
        }
        Lnode *new = malloc(sizeof(Lnode));
        new->id = id;
        new->next = NULL;
        p->next = new;
    }
}

void clearUnref(Lnode* unref,int ref)
{
    for(Lnode* p=unref;p!=NULL;p=p->next)
    {
        WORD* word=&(vocab[p->id]);
        word->component[word->comp_len++]=ref;
    }
}

void add2ref(int com,int ref)
{
    vocab[com].component[vocab[com].comp_len++]=ref;
}

void updateChild(Node* child,int id,bool flag)
{
    if(flag) {
        child->ref=id;
        clearUnref(child->unref,id);
    }
    else{
        if(child->ref!=-1)
            add2ref(id,child->ref);
        else
            add2List(child->unref,id);
    }
}

Node* AddChild(char* word,Node* parent,int id,bool flag) {
    unsigned int hash, length = strlen(word) + 1;
    if (length > MAX_STRING) length = MAX_STRING;
    ///initialize child node
    Node* child=(Node*)malloc(sizeof(Node));
    child->word = (char *)calloc(length, sizeof(char));
    strcpy(child->word, word);

    if(flag) {
        child->ref = id;
        child->unref=NULL;
    }
    else {
        child->ref = -1;
        child->unref = (Lnode *) malloc(sizeof(Lnode));
        child->unref->id = id;
        child->unref->next = NULL;
    }
    child->child_hash=NULL;//delay assign to conquer space consumption
    child->child_list=NULL;
    child->child_size=0;

    ////////////////////////////////////////////////
    if(parent->child_list==NULL) {
        if (parent->word == NULL)//head node
        {
            parent->child_hash = (int *) calloc(child_first_layer, sizeof(int));
            int* hash=parent->child_hash;
            for(int i=0;i<child_first_layer;i++) hash[i]=-1;
            //memset(parent->child_hash,-1,child_first_layer);
        }
        else {
            parent->child_hash = (int *) calloc(child_hash_size, sizeof(int));
            int* hash=parent->child_hash;
            for(int i=0;i<child_hash_size;i++) hash[i]=-1;
            //memset(parent->child_hash,-1,child_first_layer);
        }
        ////////////////////////////////////////////////

        parent->child_list = (Node **) malloc(MAX_CHILD * sizeof(Node*));
        parent->max_size=MAX_CHILD;
    }

    parent->child_list[parent->child_size++]=child;

    // Reallocate memory if needed
    if (parent->child_size + 2 >= parent->max_size) {
        parent->max_size += 1000;
        parent->child_list = (Node **)realloc(parent->child_list, parent->max_size * sizeof(Node*));
    }
    if(parent->word==NULL) {
        hash = GetWordHash_head(word);
        while (parent->child_hash[hash] != -1)
            hash = (hash + 1) % child_first_layer;
    }
    else{
        hash = GetWordHash_other(word);
        while (parent->child_hash[hash] != -1) hash = (hash + 1) % child_hash_size;
    }
    parent->child_hash[hash] = parent->child_size - 1;

    return child;
}


Node* initTrie()
{
    Node* root=(Node*)malloc(sizeof(Node));
    root->word=NULL;
    root->child_hash=NULL;
    root->child_list=NULL;
    root->ref=-1;
    root->child_size=0;
    return root;
}

void add2Trie(Node* root,int id)
{
    int count=vocab[id].wc;
    if(count==0) {
        vocab[id].words=(char**)malloc(sizeof(char*));
        vocab[id].words[0]=vocab[id].word;
    }
    char** words=vocab[id].words;
    Node* ptr[count+1];
    for(int i=0;i<=count;i++)
        ptr[i]=root;
    for(int i=0;i<=count;i++) {
        for(int j=0;j<=i;j++) {
            Node* child=SearchChild(words[i],ptr[j]);
            if(child==NULL) {
                if ((i == count) && (j == 0)) ptr[j]=AddChild(words[i], ptr[j], id, true);
                else ptr[j]=AddChild(words[i], ptr[j], id, false);
            }
            else {
                if ((i == count) && (j == 0)) updateChild(child, id, true);
                else updateChild(child, id, false);
                ptr[j]=child;
            }
        }
    }
}

