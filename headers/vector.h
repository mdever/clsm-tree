#ifndef VECTOR_H
#define VECTOR_H

#include "record.h"
#define INITIAL_SIZE 1024

typedef struct vector_head {
    int capacity;
    int current_size;
    word_record_t *start;
} vector_head_t;

vector_head_t *make_vector();
int vector_insert(vector_head_t *head, word_record_t value);
word_record_t *vector_find(vector_head_t *head, char *str);


#endif // VECTOR_H