#include "record.h"
#include "vector.h"
#include <memory.h>

int compare_word_records(void *first, void *second)
{
  word_record_t *_first = (word_record_t *) first;
  word_record_t *_second = (word_record_t *) second;
  return strcmp(_first->word, _second->word);
}

int find_word_record(void *first, void *second)
{
  word_record_t *_first = (word_record_t *) first;
  char * _second = (char *) second;
  return strcmp(_first->word, _second);
}

vector_head_t *make_vector()
{
    vector_head_t *head = (vector_head_t *) malloc(sizeof(vector_head_t));
    head->capacity = 1024;
    head->start = (word_record_t *) malloc(sizeof(word_record_t) * 1024);
    head->current_size = 0;
    return head;
}

int vector_insert(vector_head_t *head, word_record_t value)
{
    if (head->current_size >= head->capacity) {
        int old_size = head->capacity;
        head->capacity *= 2;
        word_record_t *old = head->start;
        head->start = (word_record_t *) malloc(sizeof(word_record_t) * head->capacity);
        memcpy(head->start, old, old_size * sizeof(word_record_t));
        free(old);
    }
    memcpy(&head->start[head->current_size], &value, sizeof(word_record_t)); // Copy it in
    head->current_size++;
    return head->current_size;
}

word_record_t *vector_find(vector_head_t *head, char *str)
{
    for (int i = 0; i < head->current_size; i++) {
        word_record_t *record = &head->start[i];
        if (strcmp(record->word, str) == 0) {
            return record;
        }
    }

    return NULL;
}