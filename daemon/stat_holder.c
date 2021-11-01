#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stat_holder.h"

struct stat_device_t* new_stat_device(char *key)
{
    struct stat_device_t *p = malloc(sizeof(struct stat_device_t));
    if (p == NULL)
        return NULL;
    p->key = key;
    p->t = new_trie();
    p->next = NULL;
    return p;
}

void del_stat_device(struct stat_device_t* d)
{
    if (!d) return;
    del_stat_device(d->next);
    del_trie(d->t);
    free(d->key);
    free(d);
}

stat_holder* new_stat_holder()
{
    stat_holder *h = calloc(1, sizeof(stat_holder));
    return h;
}

void del_stat_holder(stat_holder* h)
{
    del_stat_device(h->current);
    del_stat_device(h->begin);
    free(h);
}

int stat_holder_cnt_devs(stat_holder* h)
{
    int res = 0;
    struct stat_device_t* tmp_dev = h->begin;
    while (tmp_dev) {
        tmp_dev = tmp_dev->next;
        ++res;
    }
    return res;
}

struct stat_device_t* stat_holder_add_dev(stat_holder *h, char *dev_name)
{
    struct stat_device_t* tmp_dev = h->begin;
    if (!tmp_dev)
        return h->begin = h->current = new_stat_device(dev_name);

    while (tmp_dev->next != NULL) {
        if (!strcmp(tmp_dev->key, dev_name))
            return tmp_dev;
        tmp_dev = tmp_dev->next;
    }
    return tmp_dev->next = new_stat_device(dev_name);
}

//
//
//struct stat_device_compressed_t {
//    int key_len, trie_len;
//    char *key, *trie_compressed;
//};
//
//struct stat_holder_compressed_t {
//    int cnt_dev, id_current, data_len;
//    char *data;
//};
//
//typedef struct stat_device_compressed_t stat_device_compressed;
//typedef struct stat_holder_compressed_t stat_holder_compressed;
//
//stat_device_compressed *new_stat_device_compressed();
//void stat_device_compressed_del(stat_device_compressed* dc);
//
//stat_holder_compressed* trie_dump(stat_holder *h);
//stat_holder* trie_load(stat_holder_compressed* hc);
//
//stat_device_compressed *new_stat_device_compressed() {
//    stat_device_compressed *d = malloc(sizeof(stat_device_compressed));
//    d->key_len = 0;
//    d->trie_len = 0;
//    d->key = NULL;
//    d->trie_compressed = NULL;
//    return d;
//}
//
//void stat_device_compressed_del(stat_device_compressed* dc) {
//    if (dc->trie_compressed) free(dc->trie_compressed);
//    if (dc->key) free(dc->key);
//    free(dc);
//}
//
//stat_holder_compressed* new_stat_holder_compressed() {
//    return calloc(sizeof(stat_holder_compressed));
//}
//
//stat_holder_compressed* stat_holder_dump(stat_holder* h) {
//    stat_holder_compressed* hc = new_stat_holder_compressed();
//    int n = stat_holder_cnt_devs(h);
//
//}
