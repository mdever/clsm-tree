#include "common.h"
#include "rb.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

char *serialize(record_t *record)
{
  int bufsize = 1024;
  char *valbuf = (char *) malloc(sizeof(char) * bufsize);
  memset(valbuf, 0, bufsize);
  valbuf[0] = '\0';

  strcpy(valbuf, record->key);
  int keylen = strlen(valbuf);
  valbuf[keylen] = ':';
  valbuf[keylen+1] = '\0';
  for (int i = 0; i < strlen(record->value); i++) {
    char nextChar[3];
    nextChar[0] = '\0';
    if (record->value[i] == ';') {
      nextChar[0] = '\\';
      nextChar[1] = ';';
      nextChar[2] = '\0';
    } else {
      nextChar[0] = record->value[i];
      nextChar[1] = '\0';
    }

    if (strlen(valbuf)+3+1 > bufsize) {
      char *prevbuf = valbuf;
      bufsize *= 2;
      valbuf = (char *) malloc(sizeof(char) * bufsize);
      memset(valbuf, 0, bufsize);
      strcpy(valbuf, prevbuf);
      free(prevbuf);
    }

    strcat(valbuf, nextChar);
  }
}

void _serialize_tree(rb_node_t *node, char **buffer, int *curlen, int *max)
{
  record_t *record = (record_t *) node->data;
  int mylen = (strlen(record->key) + 1 + strlen(record->value)) * 2;
  if (node->left != NULL) {
    _serialize_tree(node->left, buffer, curlen, max);
  }

  if (*curlen + mylen > max) {
    char *prevbuf = *buffer;
    *max *= 2;
    *buffer = (char *) malloc(sizeof(char) * *max);
    strcpy(*buffer, prevbuf);
    free(prevbuf);
  }

  char *currkv = serialize(record);
  mylen = strlen(currkv);
  strcpy(*buffer, currkv);
  free(currkv);

  *curlen += mylen+1;

  if (node->right != NULL) {
    _serialize_tree(node->right, buffer, curlen, max);
  }
}

char *serialize_tree(rb_tree_t *tree)
{
  int size = 4096;
  char *buffer = (char *) malloc(sizeof(char) * size);
  memset(buffer, 0, size);
  int len = 0;
  buffer[0] = '\0';
  if (tree->root) {
    _serialize_tree(tree->root, &buffer, &len, &size);
    return *buffer;
  }

  return "";
}

int main(int argc, char *argv[])
{
  rb_tree_t *records = (rb_tree_t *) malloc(sizeof(rb_tree_t));
  records->compare = compare_record;
  records->find = find_record;
  records->root = NULL;
  records->count = 0;

  insert_record(records, "name", "Mark");
  insert_record(records, "age", "32");
  insert_record(records, "job", "programmer");

  rb_node_t *age = find(records, "age");
  if (age != NULL) {
    record_t *record = (record_t *) age->data;
    printf("Found the variable %s and the value is %s.\n", "age", record->value);
  } else {
    printf("Could not find key age.\n");
  }

  printf("Attempting to serialize...\n");
  char *ser = serialize_tree(records);
  printf("\n\n%s\n\n", ser);
  free(ser);
  return 0;
}