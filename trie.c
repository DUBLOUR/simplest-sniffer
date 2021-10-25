#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct trie_node {
    int value;
    struct trie_node* next[2];
};

struct trie_node* new_node() {
    struct  trie_node *node = malloc(sizeof(struct trie_node));
    if (node == NULL)
        return NULL;

    node->value = 0;
    node->next[0] = NULL;
    node->next[1] = NULL;
    return node;
}

struct trie_entrypoint {
    char *key;
    struct trie_node *node;
    struct trie_entrypoint* next;
};

struct trie_entrypoint* new_entypoint(char *key) {
    struct trie_entrypoint *p = malloc(sizeof(struct trie_entrypoint));
    if (p == NULL)
        return NULL;
    p->key = key;
    p->node = new_node();
    p->next = NULL;
    return p;
}

struct trie {
    struct trie_entrypoint* root;
};

void _trie_increase(struct trie_node* v, void *data, int bit_pos) {

    v->value++;
    if (bit_pos<0)
        return;

    bool turn = false;
    char *byte = ((char*) data) + (bit_pos>>3);
    char p = 1<<(bit_pos&7);
    turn = !!((*byte)&p);
    printf("_t_inc %d %d\n", bit_pos, turn);
//    turn//!!(char(*(data+(bit>>3)))&(1<<(bit&7)));
    if (v->next[turn] == NULL)
        v->next[turn] = new_node();
    _trie_increase(v->next[turn], data, bit_pos-1);
}

int trie_add_root(struct trie* t, char* key) {
    struct trie_entrypoint* r = t->root;
    if (r == NULL) {
        t->root = new_entypoint(key);
    } else {
        while (r->next != NULL)
            r = r->next;
        (*r).next = new_entypoint(key);
    }
}

int trie_increase(struct trie* t, char* key, void *data, int cnt_bits) {
    for (struct trie_entrypoint* tmp=t->root; tmp!=NULL; tmp=tmp->next) {
        printf("t_inc %s %s\n", key, tmp->key);
        if (!strcmp(tmp->key, key)) {
            _trie_increase(tmp->node, data, cnt_bits-1);
            return 0;
        }
    }
    return -1;
}


int _trie_get(struct trie_node* v, void *data, int bit) {
    if (v == NULL)
        return 0;
    if (!bit)
        return v->value;

    bool turn = false;
    char *byte = ((char*) data) + (bit>>3);
    char p = 1<<(bit&7);
    turn = !!((*byte)&p);

    return _trie_get(v->next[turn], data, bit-1);
}

int trie_get(struct trie* t, char* key, void *data, int cnt_bits) {
    for (struct trie_entrypoint* tmp=t->root; t!=NULL; t++)
        if (!strcmp(tmp->key, key))
            return _trie_get(tmp->node, data, cnt_bits);
    return 0;
}

void _trie_walkthrough(struct trie_node* v, int shift) {
    for (int i=0; 0&&i<shift; ++i)
        printf("     ");
    if (v == NULL) {
        printf("-(*)\n");
        return;
    }

    printf("-(%d)", v->value);
    if (v->next[0] == NULL && v->next[1] == NULL) {
        printf("\n");
        return;
    }
    if (v->next[0] != NULL && v->next[1] != NULL) {
        printf("--0-");
        _trie_walkthrough(v->next[0], shift+1);
        for (int i=0; i<shift-1; ++i)
            printf("        ");
        printf("  \\---1-");
        _trie_walkthrough(v->next[1], shift+1);
        return;
    }
    if (v->next[0] != NULL) {
        printf("--0-");
        _trie_walkthrough(v->next[0], shift+1);
    } else {
        printf("--1-");
        _trie_walkthrough(v->next[1], shift+1);
    }
//    _trie_walkthrough(v->next[0], shift+1);
//    _trie_walkthrough(v->next[1], shift+1);
//    printf("\n");
}

void trie_walkthrough(struct trie* t) {
    for (struct trie_entrypoint* r=t->root; r!=NULL; r=r->next) {
        printf("%s:\n", r->key);
        _trie_walkthrough(r->node, 0);
    }
}

int main()
{
    printf("xxx\n");

    char dev[] = "eth0";
    struct trie *t = malloc(sizeof(struct trie));
    trie_add_root(t, dev);
    unsigned ip;
    ip = 10; trie_increase(t, dev, &ip, 5);
    ip = 15; trie_increase(t, dev, &ip, 5);
    ip = 4; trie_increase(t, dev, &ip, 5);
    ip = 10; trie_increase(t, dev, &ip, 5);
//    return 0;
    ip = 5;
    printf("%d\n", trie_get(t, dev, &ip, 5));

    trie_walkthrough(t);
/**
10 15 4


10 = 2+8 = 01010
15 = 1+2+4+8 = 01111
4 = 00100
32

x --0-> x --0-> x --1-> x --0-> x --0-> x
         \
          --1-> x --0-> x --1-> x --0-> x
                 \
                  --1-> x --1-> x --1-> x


eth0:
-(4)--0--(4)--0--(4)--0--(1)--1--(1)--0--(1)
          \---1--(3)--0--(2)--1--(2)
                  \---1--(1)--1--(1)


*/
}