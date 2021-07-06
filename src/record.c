#include "record.h"

int compare_record(void *first, void *second)
{
  record_t *_first = (record_t *) first;
  record_t *_second = (record_t *) second;

  return strcmp(_first->key, _second->key);
}

int find_record(void *first, void *second)
{
  record_t *record = (record_t *) first;
  char *key = (char *) second;

  return strcmp(record->key, key);
}