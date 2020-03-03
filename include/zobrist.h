#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "board.h"
#include <stdint.h>

typedef struct entry {
    int value;
    int depth;
    uint64_t key;
    struct entry* next;
} *entry;

typedef struct z_hashtable {
    uint64_t size;
    entry* items;
    uint64_t* hashpool;
} *z_hashtable;

uint64_t* create_hashpool(int seed);
z_hashtable create_hashtable(uint64_t size, int seed);
int add_hashtable(z_hashtable z_ht, entry e);
entry find_hashtable(z_hashtable z_ht, uint64_t key);
void destroy_hashtable(z_hashtable ht);
void destroy_entry(entry e);
entry create_entry();

#endif
