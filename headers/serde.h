#ifndef SERDE_H
#define SERDE_H

#include "rb.h"

void serialize_int(unsigned int number, char *place);
unsigned int recover_int(char *serialized);
char *serialize_tree(rb_tree_t *tree, int *totallen);
rb_tree_t *deserialize(char *contents, int length);

#endif