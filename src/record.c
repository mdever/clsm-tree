#include "record.h"
#include "rb.h"

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

rb_tree_t *record_tree_create()
{
  rb_tree_t *t = (rb_tree_t *) malloc(sizeof(rb_tree_t));
  t->compare = compare_record;
  t->find = find_record;
  t->count = 0;
  t->root = NULL;
  return t;
}

void _print_records(rb_node_t *node)
{
  if (node->left != NULL)
    _print_records(node->left);

  record_t *record = (record_t *) node->data;
  printf("Key: %s, Value: %s\n", record->key, record->value);

  if (node->right != NULL)
    _print_records(node->right);
}

void print_records(rb_tree_t *tree)
{
  if (tree->root != NULL) {
    _print_records(tree->root);
  }
}
