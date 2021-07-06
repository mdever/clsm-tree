#include "hashmap.h"
#include <string.h>

int hash_key(char *key, int capacity)
{
    int value = 0;
    for (int i = 0; i < strlen(key); i++) {
        value += (int) key[i];
    }

    return value % capacity;
}

hashmap_t *hm_create()
{
    hashmap_t *hm = (hashmap_t *) malloc(sizeof(hashmap_t));
    hm->capacity = 1024;
    hm_bucket_t *buckets = (hm_bucket_t *) malloc(sizeof(hm_bucket_t) * 1024);
    memset(buckets, 0, sizeof(hm_bucket_t) * 1024);
    hm->buckets = buckets;
    hm->nbuckets = 0;

    return hm;
}

hm_bucket_t *hm_put(hashmap_t *hm, char *key, int value)
{
    int hash = hash_key(key, hm->capacity);
    if (strcmp(hm->buckets[hash].key, "") == 0) {
        strcpy(&hm->buckets[hash].key, key);
        hm->buckets[hash].value = value;
        hm->buckets[hash].next = NULL;
        return &hm->buckets[hash];
    } else if (strcmp(hm->buckets[hash].key, key) == 0) {
        hm->buckets[hash].value = value;
        return &hm->buckets[hash];
    } else {
        hm_bucket_t *bucket = &hm->buckets[hash];
        while (bucket->next != NULL) {
            bucket = bucket->next;
        }

        bucket->next = (hm_bucket_t *) malloc(sizeof(hm_bucket_t));
        bucket = bucket->next;
        strcpy(bucket->key, key);
        bucket->value = value;
        bucket->next = NULL;
        return bucket;
    }
}

int *hm_get(hashmap_t *hm, char *key)
{
    int hash = hash_key(key, hm->capacity);
    hm_bucket_t *bucket = &hm->buckets[hash];
    if (strcmp(bucket->key, "") == 0) {
        return NULL;
    }

    if (strcmp(bucket->key, key) == 0) {
        return &bucket->value;
    }

    while (bucket->next != NULL) {
        bucket = bucket->next;
        if (strcmp(bucket->key, key) == 0) {
            return &bucket->value;
        }
    }

    return NULL;
}

void hm_free(hashmap_t *hm)
{

}