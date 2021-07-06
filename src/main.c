#include "record.h"
#include "rb.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "serde.h"

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
  insert_record(records, "additionalInterests", "{\"first\": \"cycling\"}");

  rb_node_t *age = find(records, "age");
  if (age != NULL) {
    record_t *record = (record_t *) age->data;
    printf("Found the variable %s and the value is %s.\n", "age", record->value);
  } else {
    printf("Could not find key age.\n");
  }

  printf("Attempting to serialize...\n");
  int len;
  char *ser = serialize_tree(records, &len);
  FILE *f = fopen("./serialized.bin", "wb");
  if (f == NULL) {
    printf("Could not open file for writing.\n");
    return 0;
  }

  fwrite(ser, sizeof(char), len, f);
  fclose(f);
  free(ser);

  f = fopen("./serialized.bin", "rb");
  fseek(f, 0L, SEEK_END);
  int sz = ftell(f);
  fseek(f, 0L, SEEK_SET);
  char *contents = (char *) malloc(sizeof(char) * sz);
  memset(contents, 0, sz);

  fread(contents, 1, sz, f);

  rb_tree_t *t = deserialize(contents, sz);
  print_records(t);

  return 0;
}