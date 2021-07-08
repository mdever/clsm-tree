#ifndef SERDE_H
#define SERDE_H

#include "rb.h"
#include "record.h"

void *serialize(record_t *record, int *size);
void serialize_int(unsigned int number, unsigned char *place);
unsigned int recover_int(unsigned char *serialized);
char *serialize_tree(rb_tree_t *tree, int *totallen);
rb_tree_t *deserialize(char *contents, int length);

#endif