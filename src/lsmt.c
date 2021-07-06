#include "lsmt.h"
#include "record.h"
#include "rb.h"
#include "hashmap.h"
#include "serde.h"
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

bool lsmtree_create(lsmtree_t *lsm, char *basedir)
{
    strcpy(&lsm->basedir, basedir);
    lsm->hm.capacity = 1024;
    lsm->hm.nbuckets = 0;
    lsm->hm.buckets = (hm_bucket_t *) malloc(sizeof(hm_bucket_t) * 1024);
    lsm->rb.count = 0;
    lsm->rb.root = NULL;
    lsm->rb.compare = compare_record;
    lsm->rb.find = find_record;

    mkdir(basedir);

    return true;
}
// bool lsmtree_open(char *basedir, lsmtree *lsm);
// bool lsmtree_put(lsmtree *lsm, char *key, char *value);
// char *lsmtree_get(lsmtree *lsm, char *key);
// bool lsmtree_remove(lsmtree *lsm, char *key);
// bool lsmtree_close(lsmtree *lsm);
// bool lsmtree_destroy(lsmtree *lsm);
// bool lsmtree_flush(lsmtree *lsm);