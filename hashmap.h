#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct hm_bucket {
    char key[64];
    int value;
    struct hm_bucket *next;
} hm_bucket_t;

typedef struct hashmap {
    hm_bucket_t *buckets;
    int capacity;
    int nbuckets;
} hashmap_t;

hashmap_t *hm_create();
hm_bucket_t *hm_put(hashmap_t *hm, char *key, int value);
int *hm_get(hashmap_t *hm, char *key);
void hm_free(hashmap_t *hm);

#endif