#ifndef LSMT_H
#define LSMT_H
#include "record.h"
#include "rb.h"
#include "hashmap.h"
#include "serde.h"
#include "bloom.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct lsm_config {
    char* files[64];
} lsm_config_t;

typedef struct lsmtree {
    hashmap_t hm;
    rb_tree_t rb;
    bloom_filter_t bloom_filter;
    FILE *wal_file;
    FILE *segments[8];
    char basedir[256];
    unsigned int nsegments;
} lsmtree_t;

bool lsmtree_create(lsmtree_t *lsm, char *basedir);
bool lsmtree_open(lsmtree_t *lsm, char *basedir);
bool lsmtree_put(lsmtree_t *lsm, char *key, char *value);
char *lsmtree_get(lsmtree_t *lsm, char *key);
bool lsmtree_remove(lsmtree_t *lsm, char *key);
bool lsmtree_close(lsmtree_t *lsm);
bool lsmtree_destroy(lsmtree_t *lsm);
bool lsmtree_flush(lsmtree_t *lsm);



#endif LSMT_H