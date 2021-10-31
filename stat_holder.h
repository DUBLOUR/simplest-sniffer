#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

struct stat_device_t {
    char *key;
    trie *t;
    struct stat_device_t* next;
};

struct stat_holder_t {
    struct stat_device_t *begin, *current;
};

typedef struct stat_holder_t stat_holder;

struct stat_device_t* new_stat_device(char *key);
void del_stat_device(struct stat_device_t* d);
stat_holder* new_stat_holder();
struct stat_device_t* stat_holder_add_dev(stat_holder *h, char *dev_name);