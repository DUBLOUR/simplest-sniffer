#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

struct trie_node_t {
    int value;
    struct trie_node_t* children[2];
};

struct trie_t {
    struct trie_node_t* root;
    pthread_mutex_t mutex;
};

typedef struct trie_t trie;

trie* new_trie();
void del_trie(trie* t);
void trie_increase_val(trie* t, void *path, int path_len_bits, int inc_val);
void trie_increase(trie* t, void *path, int path_len_bits);
int trie_get(trie* t, void *path, int path_len_bits);
void trie_traversing(trie* t);


struct trie_compressed_t {
    int cnt_point, data_len;
    char *data;
};

struct point_compressed_t {
    int value, path_len_bit;
    void *path;
};

typedef struct trie_compressed_t trie_compressed;
typedef struct point_compressed_t point_compressed;

trie_compressed *new_trie_compressed();
void del_trie_compressed(trie_compressed* tc);

trie_compressed* trie_dump(trie *t);
trie* trie_load(trie_compressed* tc);