#include "record.h"
#include "rb.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "serde.h"
#include "lsmt.h"

int main(int argc, char *argv[])
{

  lsmtree_t lsm;
  lsmtree_create(&lsm, "./db");
  lsmtree_put(&lsm, "name", "Mark");
  lsmtree_put(&lsm, "age", "32");
  lsmtree_put(&lsm, "name", "Mark Disbrow");
  lsmtree_close(&lsm);

  FILE *f = fopen("./db/wal.log", "rb");
  size_t len;
  fseek(f, 0, SEEK_CUR);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buf = (char *) malloc(sizeof(char) * len);
  fread(buf, sizeof(char), len, f);
  rb_tree_t *t = deserialize(buf, len);
  fclose(f);

  print_records(t);


  return 0;
}