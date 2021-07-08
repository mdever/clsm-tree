#ifndef RB_H
#define RB_H
#include <stdbool.h>

enum rb_color {
  RED,
  BLACK,
};

struct rb_tree;

typedef struct rb_node {
  struct rb_tree *tree;
  struct rb_node *parent;
  struct rb_node *left;
  struct rb_node *right;
  enum rb_color color;
  void *data;
} rb_node_t;

typedef struct rb_tree {
  int (*compare)(void *left, void *right);
  int (*find)(void *first, void *second);
  struct rb_node *root;
  int count;
} rb_tree_t;

rb_node_t *rb_insert(rb_tree_t *tree, void *data);
void *rb_delete(rb_tree_t *tree, void *data);
void rb_print_in_order(rb_tree_t *tree);
rb_node_t *rb_find(rb_tree_t *tree, void *value);

#endif
