#include "serde.h"
#include "common.h"
#include "rb.h"
#include <string.h>

void serialize_int(unsigned int number, char *place)
{
  unsigned char byte1 = (number & 0xff);
  unsigned char byte2 = (number & 0xff00) >> 8;
  unsigned char byte3 = (number & 0xff0000) >> 16;
  unsigned char byte4 = (number & 0xff000000) >> 24;
  sprintf(place, "%c%c%c%c", byte1, byte2, byte3, byte4);
}

unsigned int recover_int(char *serialized)
{
  unsigned int out;
  unsigned char byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
  unsigned int int1 = 0, int2 = 0, int3 = 0, int4 = 0;
  sscanf(serialized, "%c%c%c%c", &byte1, &byte2, &byte3, &byte4);
  int1 = ((unsigned int)  byte1)        & 0x000000ff;
  int2 = (((unsigned int) byte2) << 8)  & 0x0000ff00;
  int3 = (((unsigned int) byte3) << 16) & 0x00ff0000;
  int4 = (((unsigned int) byte4) << 24) & 0xff000000;
  out = int4 | int3 | int2 | int1;
  return out;
}

char *serialize(record_t *record, int *size)
{
  int keysize = strlen(record->key);
  int valsize = strlen(record->value);
  int totalsize = keysize + valsize + 4 + 4;
  char *buffer = (char *) malloc(sizeof(char) * totalsize);
  memset(buffer, 0, totalsize);
  char ckeysize[4];
  char cvalsize[4];
  
  serialize_int(keysize, ckeysize);
  serialize_int(valsize, cvalsize);

  memcpy(buffer, ckeysize, 4);
  memcpy(buffer+4, cvalsize, 4);
  memcpy(buffer+8, record->key, keysize);
  memcpy(buffer+8+keysize, record->value, valsize);

  *size = totalsize;
  return buffer;
}

void _serialize_tree(rb_node_t *node, char **buffer, int *curlen, int *max)
{
  if (node->left != NULL) {
    _serialize_tree(node->left, buffer, curlen, max);
  }

  record_t *record = (record_t *) node->data;
  int mylen;
  char *currkv = serialize(record, &mylen);
  if (*curlen + mylen > max) {
    char *prevbuf = *buffer;
    int prevmax = *max;
    *max *= 2;
    *buffer = (char *) malloc(sizeof(char) * *max);
    memset(*buffer, 0, sizeof(char) * *max);
    memcpy(*buffer, prevbuf, prevmax);
    free(prevbuf);
  }

  memcpy((void *) (*buffer+*curlen), currkv, mylen);
  free(currkv);

  *curlen += mylen;

  if (node->right != NULL) {
    _serialize_tree(node->right, buffer, curlen, max);
  }
}

char *serialize_tree(rb_tree_t *tree, int *totallen)
{
  int size = 4096;
  char *buffer = (char *) malloc(sizeof(char) * size);
  memset(buffer, 0, size);
  int len = 0;
  buffer[0] = '\0';
  if (tree->root) {
    _serialize_tree(tree->root, &buffer, &len, &size);
    *totallen = len;
    return buffer;
  }

  return "";
}

rb_tree_t *deserialize(char *contents, int length)
{
    unsigned int keylen;
    unsigned int vallen;
    unsigned int nextentry = 0;
    rb_tree_t *records = (rb_tree_t *) malloc(sizeof(rb_tree_t));
    records->compare = compare_record;
    records->find = find_record;
    records->root = NULL;
    records->count = 0;

    while (nextentry < length) {
        keylen = recover_int((char *) &contents[nextentry]);
        vallen = recover_int((char *) &contents[nextentry+4]);
        char *key = (char *) malloc(sizeof(char) * (keylen+1));
        char *value = (char *) malloc(sizeof(char) * (vallen+1));
        memcpy(key, (char *) &contents[nextentry+8], keylen);
        key[keylen] = '\0';
        memcpy(value, (char *) &contents[nextentry+8+keylen], vallen);
        value[vallen] = '\0';
        insert_record(records, key, value);
        nextentry += 8 + keylen + vallen;
    }

    return records;
}