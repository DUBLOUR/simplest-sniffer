#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "trie.h"


struct trie_node_t* new_node()
{
    struct  trie_node_t *node = malloc(sizeof(struct trie_node_t));
    if (node == NULL)
        return NULL;

    node->value = 0;
    node->children[0] = NULL;
    node->children[1] = NULL;
    return node;
}

void del_trie_node(struct trie_node_t* v)
{
    if (!v) return;
    del_trie_node(v->children[0]);
    del_trie_node(v->children[1]);
    free(v);
}

trie* new_trie()
{
    trie *t = malloc(sizeof(trie));
    t->root = new_node();
    pthread_mutex_init(&(t->mutex), NULL);
    return t;
}

void del_trie(trie* t)
{
    del_trie_node(t->root);
    free(t);
}

bool get_ip_turn(char *data, int data_len, int pos)
{
    data_len >>= 3;
    char *byte = data + (data_len-1 - (pos>>3));
    char bit = 1<<(7-(pos&7));
    return !!(*byte & bit);
}

bool get_turn(char *data, int data_len, int pos)
{
    return (data[pos>>3] >> (pos&7))&1;
}

void trie_increase_val(trie* t, void *path, int path_len_bits, int inc_val)
{
    pthread_mutex_lock(&(t->mutex));

    struct trie_node_t* v = t->root;
    for (int pos=0; pos<path_len_bits; ++pos) {
        v->value += inc_val;

        bool turn = get_turn(path, path_len_bits, pos);
        if (v->children[turn] == NULL)
            v->children[turn] = new_node();
        v = v->children[turn];
    }
    v->value += inc_val;

    pthread_mutex_unlock(&(t->mutex));
}

void trie_increase(trie* t, void *path, int path_len_bits)
{
    trie_increase_val(t, path, path_len_bits, 1);
}

int trie_get(trie* t, void *path, int path_len_bits)
{
    pthread_mutex_lock(&(t->mutex));

    struct trie_node_t* v = t->root;
    for (int pos=0; pos<path_len_bits; ++pos) {
        bool turn = get_turn(path, path_len_bits, pos);
        v = v->children[turn];
        if (!v)
            break;
    }
    int res = v ? v->value : 0;

    pthread_mutex_unlock(&(t->mutex));
    return res;
}

void _trie_traversing(struct trie_node_t* v, int shift, int sum)
{
    if (v == NULL) {
        printf("-(*)\n");
        return;
    }

    printf("-(*)");
    shift += 8;
    //no sons
    if (v->children[0] == NULL && v->children[1] == NULL) {
        printf(": %d (%X)\n", v->value, sum);
        return;
    }
    //tho sons
    if (v->children[0] != NULL && v->children[1] != NULL) {
        printf("--0-");
        _trie_traversing(v->children[0], shift, sum*2);
        for (int i=0; i<shift-6; ++i)
            printf(" ");
        printf("\\---1-");
        _trie_traversing(v->children[1], shift, sum*2+1);
        return;
    }
    //only one son
    if (v->children[0] != NULL) {
        printf("--0-");
        _trie_traversing(v->children[0], shift, sum*2);
    } else {
        printf("--1-");
        _trie_traversing(v->children[1], shift, sum*2+1);
    }
}

void trie_traversing(trie* t)
{
    pthread_mutex_lock(&(t->mutex));

    _trie_traversing(t->root, 0, 0);

    pthread_mutex_unlock(&(t->mutex));
}


void traversing_dump(struct trie_node_t* t, int pos, int* total_size, int* cnt_node, char* bit_path, void* data)
{
    if (t->children[0]==NULL && t->children[1]==NULL) {
        (*cnt_node)++;
        int path_len = (pos|7)>>3;
        if (!data) {
            *total_size += sizeof(point_compressed) + sizeof(char)*path_len;
            return;
        }
        point_compressed* p = (point_compressed*) data + *total_size;
        p->value = t->value;
        p->path_len_bit = pos;
        p->path = NULL;
        *total_size += sizeof(point_compressed);
        memcpy(data + *total_size, bit_path, sizeof(char)*path_len);
        printf("td %d %d %X\n", p->value, p->path_len_bit, *((unsigned*)bit_path) );
        *total_size += sizeof(char)*path_len;
        return;
    }

    bit_path[pos>>3] &= 0xff ^ (1<<(pos&7));
    for (int i=0; i<2; ++i)
        if (t->children[i]) {
            bit_path[pos>>3] |= i<<(pos&7);
            traversing_dump(t->children[i], pos + 1, total_size, cnt_node, bit_path, data);
        }
}

trie_compressed *new_trie_compressed()
{
    trie_compressed *tc = malloc(sizeof(trie_compressed));
    tc->cnt_point = 0;
    tc->data_len = 0;
    tc->data = NULL;
    return tc;
}

void del_trie_compressed(trie_compressed* tc)
{
    if (tc->data)
        free(tc->data);
    free(tc);
}


trie_compressed* trie_dump(trie *t)
{
    char tmp_path[1024];
    trie_compressed* tc = new_trie_compressed();

    pthread_mutex_lock(&(t->mutex));

    traversing_dump(t->root, 0, &(tc->data_len), &(tc->cnt_point), tmp_path, NULL);
    tc->data = malloc(sizeof(char)*(tc->data_len));
    int sz = 0, node_cnt = 0;
    traversing_dump(t->root, 0, &sz, &node_cnt, tmp_path, tc->data);

    pthread_mutex_unlock(&(t->mutex));
    return tc;
}

trie* trie_load(trie_compressed* tc)
{
    trie* t = new_trie();

    void* vp = tc->data;
    for (int i=0; i<tc->cnt_point; i++) {
        point_compressed* p = (point_compressed*) vp;
        trie_increase_val(t, vp+sizeof(point_compressed), p->path_len_bit, p->value);
        size_t point_size = sizeof(point_compressed) +
                            sizeof(char) * ((p->path_len_bit|7)>>3);
        vp += point_size;
    }
    return t;
}


