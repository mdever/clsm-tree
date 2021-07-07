#ifndef BLOOM_H
#define BLOOM_H

#include <stdbool.h>

typedef struct bloom_filter {

} bloom_filter_t;

bool bloom_create(bloom_filter_t *bloom);
void bloom_put(bloom_filter_t *bloom, char *key);
bool bloom_test(bloom_filter_t *bloom, char *key);

#endif