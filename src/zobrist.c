#include "zobrist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// rand function to init hash pool
static uint64_t get64rand() {
    return
        (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
        (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) |
        (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
        (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);
}

uint64_t* create_hashpool(int seed) {
    srand(seed);
    uint64_t* hashpool = malloc(sizeof(uint64_t) * 1450);
    for (int i = 0; i < 1450; i++) {
        hashpool[i] = get64rand();
    }
    return hashpool;
}

z_hashtable create_hashtable(uint64_t size, int seed) {
    z_hashtable z_ht = calloc(1, sizeof(struct z_hashtable));
    z_ht->size = size;
    z_ht->items = calloc(size, sizeof(entry));
    z_ht->hashpool = create_hashpool(seed);
    return z_ht;
}

entry create_entry() {
    entry e = calloc(1, sizeof(struct entry));
    return e;
}

void destroy_entry(entry e) {
    if (!e) {
        return;
    }
    destroy_entry(e->next);
    free(e);
}

void destroy_hashtable(z_hashtable z_ht) {
    if (!z_ht) {
        return;
    }
    for (uint64_t i = 0; i < z_ht->size; i++) {
        destroy_entry(z_ht->items[i]);
    }
    free(z_ht->hashpool);
    free(z_ht->items);
    free(z_ht);
}

/*
 * The way this is handled is susceptible to memory leaks via node losses when multithreading
 * Since there is no lock, two threads could try to add a node in the same bucket at the same time
 * The second node added would then overwrite the reference to the first one, loosing it in the process
 * This effect should be reduced when increasing the size of the hashtable and having a good hashpool
 */
int add_hashtable(z_hashtable z_ht, entry e) {
    assert(e != NULL);
    uint64_t bucket = e->key % z_ht->size;
    if (z_ht->items[bucket] == NULL) {
        z_ht->items[bucket] = e;
        return 0;
    }
    entry node = z_ht->items[bucket];
    if (node->key == e->key) {
        if (node->depth < e->depth) {
            node->depth = e->depth;
            node->value = e->value;
        }
        destroy_entry(e);
        return 1;
    }
    while (node->next != NULL) {
        node = node->next;
        if (node->key == e->key) {
            if (node->depth < e->depth) {
                node->depth = e->depth;
                node->value = e->value;
            }
            destroy_entry(e);
            return 1;
        }
    }
    node->next = e;
    return 2;
}

entry find_hashtable(z_hashtable z_ht, uint64_t key) {
    entry node = z_ht->items[key % z_ht->size];
    if (!node) {
        return NULL;
    }
    if (node->key == key) {
        return node;
    }
    while (node->next) {
        node = node->next;
        if (node->key == key) {
            return node;
        }
    }
    return NULL;
}
