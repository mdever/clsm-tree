#ifndef COMMON_H
#define COMMON_H
#include <string.h>

typedef struct word_record {
  char *word;
  int count;
} word_record_t;

int compare_word_records(void *first, void *second);

int find_word_record(void *first, void *second);

typedef struct record {
  char key[128];
  char *value;
} record_t;

int compare_record(void *first, void *second);
int find_record(void *first, void *second);

#endif // COMMON_H

