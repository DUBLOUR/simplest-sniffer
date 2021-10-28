#include <stdlib.h>
#include <stdbool.h>

struct trie_node {
    int value;
    struct trie_node* next[2];
};

struct trie_node* new_node();

struct trie_entrypoint {
    char *key;
    struct trie_node *node;
    struct trie_entrypoint* next;
};

struct trie_entrypoint* new_entypoint(char *key);

struct trie {
    struct trie_entrypoint* root;
};

struct trie_entrypoint* trie_add_root(struct trie* t, char* key);
bool _trie_turn(char *data, int data_len, int pos);
void _trie_increase(struct trie_node* v, char *data, int data_len, int pos);
void trie_increase(struct trie* t, char* key, void *data, int cnt_bytes);
int _trie_get(struct trie_node* v, char *data, int data_len, int pos);
int trie_get(struct trie* t, char* key, void *data, int cnt_bytes);
void _trie_traversing(struct trie_node* v, int shift, int sum);
void trie_traversing(struct trie* t);