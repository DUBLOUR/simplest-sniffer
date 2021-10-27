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


struct trie_entrypoint* trie_add_root(struct trie* t, char* key) {
    struct trie_entrypoint* r = t->root;
    if (r == NULL) {
        t->root = new_entypoint(key);
        return t->root;
    }
    while (r->next != NULL)
        r = r->next;
    r -> next = new_entypoint(key);
    return r->next;
}


bool _trie_turn(char *data, int data_len, int pos) {
    char *byte = data + (data_len-1 - (pos>>3));
    char bit = 1<<(7-(pos&7));
    return !!(*byte & bit);
}

void _trie_increase(struct trie_node* v, char *data, int data_len, int pos) {
    v->value++;
    if (pos == data_len*8)
        return;

    bool turn = _trie_turn(data, data_len, pos);

//    printf("_t_inc %d %d\n", pos, turn);
    if (v->next[turn] == NULL)
        v->next[turn] = new_node();
    _trie_increase(v->next[turn], data, data_len, pos+1);
}

void trie_increase(struct trie* t, char* key, void *data, int cnt_bytes) {
    for (struct trie_entrypoint* tmp=t->root; tmp!=NULL; tmp=tmp->next) {
//        printf("t_inc %s %s\n", key, tmp->key);
        if (!strcmp(tmp->key, key)) {
            _trie_increase(tmp->node, data, cnt_bytes, 0);
            return;
        }
    }

    struct trie_entrypoint* new_entry = trie_add_root(t, key);
    _trie_increase(new_entry->node, data, cnt_bytes, 0);
}

int _trie_get(struct trie_node* v, char *data, int data_len, int pos) {
    if (v == NULL)
        return 0;
    if (pos == data_len*8)
        return v->value;

    bool turn = _trie_turn(data, data_len, pos);
    return _trie_get(v->next[turn], data, data_len, pos+1);
}

int trie_get(struct trie* t, char* key, void *data, int cnt_bytes) {
    for (struct trie_entrypoint* tmp=t->root; t!=NULL; t++)
        if (!strcmp(tmp->key, key))
            return _trie_get(tmp->node, data, cnt_bytes, 0);
    return 0;
}

void _trie_traversing(struct trie_node* v, int shift, int sum) {
    if (v == NULL) {
        printf("-(*)\n");
        return;
    }

    char s[10];
    sprintf(s, "-(*)");
    printf("%s", s);
    shift += strlen(s);

    //no sons
    if (v->next[0] == NULL && v->next[1] == NULL) {
        printf(": %d (%d)\n", v->value, sum);
        return;
    }
    //tho sons
    if (v->next[0] != NULL && v->next[1] != NULL) {
        printf("--0-");
        _trie_traversing(v->next[0], shift+4, sum*2);
        for (int i=0; i<shift-2; ++i)
            printf(" ");
        printf("\\---1-");
        _trie_traversing(v->next[1], shift+4, sum*2+1);
        return;
    }
    //only one son
    if (v->next[0] != NULL) {
        printf("--0-");
        _trie_traversing(v->next[0], shift+4, sum*2);
    } else {
        printf("--1-");
        _trie_traversing(v->next[1], shift+4, sum*2+1);
    }
}

void trie_traversing(struct trie* t) {
    for (struct trie_entrypoint* r=t->root; r!=NULL; r=r->next) {
        printf("%s:\n", r->key);
        _trie_traversing(r->node, 0, 0);
    }
}

int main()
{

    unsigned int x = 512 + 5;
    printf("%u\n", &x );
    printf("%u\n", (char*)&x );
    printf("%u\n", &x+1);
    printf("%u\n", (char*)&x+1 );
    char *p1 = (char *)&x;
    printf("%u %u\n", p1, *p1);
    printf("%u %u\n", p1-1, *(p1-1));
    printf("%u %u\n\n", p1+1, *(p1+1));
    void *p2 = &x;
    printf("%u %u\n", p2, *((char*)p2) );
    printf("%u %u\n", p2-1, *((char*)p2-1));
    printf("%u %u\n\n", p2+1, *((char*)p2+1));

//    return 0;
    printf("xxx\n");

    char dev[] = "eth0";
    struct trie *t = malloc(sizeof(struct trie));
    trie_add_root(t, dev);
    unsigned ip;
    ip = 0; trie_increase(t, dev, &ip, 1);
    ip = 90; trie_increase(t, dev, &ip, 1);
    ip = 20; trie_increase(t, dev, &ip, 1);
    ip = 3; trie_increase(t, dev, &ip, 1);
    ip = 140; trie_increase(t, dev, &ip, 1);
    ip = 5; trie_increase(t, dev, &ip, 1);
    ip = 60; trie_increase(t, dev, &ip, 1);
    ip = 13; trie_increase(t, dev, &ip, 1);
    ip = 60; trie_increase(t, dev, &ip, 1);
    ip = 9; trie_increase(t, dev, &ip, 1);
    ip = 9; trie_increase(t, dev, &ip, 1);
    ip = 10; trie_increase(t, dev, &ip, 1);

    char dev2[] = "wlan1";
    ip = 199; trie_increase(t, dev2, &ip, 2);
    ip = 205; trie_increase(t, dev2, &ip, 2);
    ip = 80; trie_increase(t, dev2, &ip, 2);
//    return 0;
    ip = 0;

    printf("%d\n", trie_get(t, dev, &ip, 1));


    trie_traversing(t);
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


eth0:
-(12)--0--(11)--0--(10)--0--(8)--0--(7)--0--(3)--0--(2)--0--(1)--0--(1): 0
            \---1--(1)--0--(1)--1--(1)--1--(1)--0--(1)--1--(1)--0--(1): 90
   \---1--(1)--0--(1)--0--(1)--0--(1)--1--(1)--1--(1)--0--(1)--0--(1): 140


*/
}