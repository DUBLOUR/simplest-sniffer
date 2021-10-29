#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

struct trie_node {
    int value;
    struct trie_node* next[2];
};

struct trie_entrypoint {
    char *key;
    struct trie_node *node;
    struct trie_entrypoint* next;
};

struct trie {
    struct trie_entrypoint* root;
    pthread_mutex_t __mutex;
};


struct trie* new_trie();
void trie_increase(struct trie* t, char* key, void *data, int cnt_bytes);
int trie_get(struct trie* t, char* key, void *data, int cnt_bytes);
void trie_traversing(struct trie* t);