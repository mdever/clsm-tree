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
#include <stdio.h>
#include <errno.h>

#if (defined(_WIN32) || defined(__WIN32__))
#define mkdir(A, B) mkdir(A)
#endif

#define FLUSH_COUNT 1024 * 1024 * 64

bool lsmtree_create(lsmtree_t *lsm, char *basedir)
{
    int dirlen = strlen(basedir);
    if (dirlen > 256) {
        return false;
    }
    strcpy(&lsm->basedir, basedir);
    memset(&lsm->segments[0], 0, 8 * sizeof(FILE *));
    lsm->hm.capacity = 1024;
    lsm->hm.nbuckets = 0;
    lsm->hm.buckets = (hm_bucket_t *) malloc(sizeof(hm_bucket_t) * 1024);
    lsm->rb.count = 0;
    lsm->rb.root = NULL;
    lsm->rb.compare = compare_record;
    lsm->rb.find = find_record;
    lsm->nsegments = 0;
    umask(0);
    mkdir(basedir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);

    char walfile_name[512];
    strcpy(walfile_name, basedir);
    if (walfile_name[dirlen-1] != '/') {
        walfile_name[dirlen] = '/';
        walfile_name[dirlen+1] = '\0';
    }

    strcat(walfile_name, "wal.log");
    FILE *walfile = fopen(walfile_name, "a+");
    if (walfile == NULL) {
        printf("Could not open %s for appending\n", walfile_name);
        printf("Error Number: %d\n", errno);
        return;
    }
    lsm->wal_file = walfile;

    return true;
}

bool lsmtree_close(lsmtree_t *lsm)
{
    //
    // Dump rb in to new segment
    //
    fclose(lsm->wal_file);
    for (int i = 0; i < lsm->nsegments; i++) {
        fclose(lsm->segments[i]);
    }

    hm_free(&lsm->hm);
}

void wal_write(lsmtree_t *lsm, char *key, char *value)
{
    record_t record;
    strcpy(&record.key, key);
    record.value = value;
    int size = 0;
    void *ser = NULL;
    ser = serialize(&record, &size);
    fwrite(ser, sizeof(char), size, lsm->wal_file);
    fflush(lsm->wal_file);
    free(ser);
}

// bool lsmtree_open(char *basedir, lsmtree *lsm);
bool lsmtree_put(lsmtree_t *lsm, char *key, char *value)
{
    bloom_put(&lsm->bloom_filter, key);
    wal_write(lsm, key, value);
    insert_record(&lsm->rb, key, value);
    if (lsm->rb.count > FLUSH_COUNT) {

    }
    return true;
}
// char *lsmtree_get(lsmtree *lsm, char *key);
// bool lsmtree_remove(lsmtree *lsm, char *key);
// bool lsmtree_close(lsmtree *lsm);
// bool lsmtree_destroy(lsmtree *lsm);
// bool lsmtree_flush(lsmtree *lsm);