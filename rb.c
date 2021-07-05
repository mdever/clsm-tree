#include "common.h"
#include "rb.h"
#include <stdlib.h>
#include <string.h>


void _left_rotate(rb_node_t *node);
void _right_rotate(rb_node_t *node);
void _fixup(rb_node_t *node);

rb_node_t *insert_record(rb_tree_t *tree, char *key, char *value)
{
  if (strlen(key) > 128) {
    perror("Key must be under 128 characters!");
    perror(key);
    return NULL;
  }

  record_t *record = (record_t *) malloc(sizeof(record_t));
  strcpy(record->key, key);
  record->value = value;

  return insert(tree, record);
}

rb_node_t *_insert(rb_node_t *node, void *data)
{
  rb_tree_t *tree = node->tree;
  int cmp = tree->compare(node->data, data);
  rb_node_t *new_node = NULL;
  if (cmp < 0) {
    if (node->right != NULL) {
      return _insert(node->right, data);
    } else {
      node->right = (rb_node_t *) malloc(sizeof(rb_node_t));
      memset(node->right, 0, sizeof(rb_node_t));
      node->right->parent = node;
      node->right->data = data;
      node->right->tree = tree;
      node->right->color = RED;
      new_node = node->right;
    }
  } else if (cmp > 0) {
    if (node->left != NULL) {
      return _insert(node->left, data);
    } else {
      node->left = (rb_node_t *) malloc(sizeof(rb_node_t));
      memset(node->left, 0, sizeof(rb_node_t));
      node->left->parent = node;
      node->left->data = data;
      node->left->tree = tree;
      node->left->color = RED;
      new_node = node->left;
    }
  } else {
    free(node->data);
    node->data = data;
    return node;
  }
  new_node->tree->count++;
  return new_node;
}

rb_node_t *insert(rb_tree_t *tree, void *data)
{
  if (tree->root == NULL) {
    tree->root = (rb_node_t *) malloc(sizeof(rb_node_t));
    memset(tree->root, 0, sizeof(rb_node_t));
    tree->root->data = data;
    tree->root->tree = tree;
    tree->root->color = BLACK;
    return tree->root;
  }

  rb_node_t *new_node = _insert(tree->root, data);
  _fixup(new_node);
  return new_node;
}

bool uncle_red(rb_node_t *node) {
    if (node->parent && node->parent->parent) {
    rb_node_t *parent = node->parent;
    rb_node_t *grandparent = node->parent->parent;
    if (parent == grandparent->left) {
      if (grandparent->right) {
        return grandparent->right->color == RED;
      }
    } else if (parent == grandparent->right) {
      if (grandparent->left) {
        return grandparent->left->color == RED;
      }
    }
  }

  return false;
}

bool uncle_black(rb_node_t *node) {
    if (node->parent && node->parent->parent) {
    rb_node_t *parent = node->parent;
    rb_node_t *grandparent = node->parent->parent;
    if (parent == grandparent->left) {
      if (grandparent->right) {
        return grandparent->right->color == BLACK;
      }
    } else if (parent == grandparent->right) {
      if (grandparent->left) {
        return grandparent->left->color == BLACK;
      }
    }
  }

  return false;
}

bool right_child(rb_node_t *node) {
  if (node->parent) {
    rb_node_t *parent = node->parent;
    if (parent->right == node) {
      return true;
    }
  }

  return false;
}

bool left_child(rb_node_t *node) {
  if (node->parent) {
    rb_node_t *parent = node->parent;
    if (parent->left == node) {
      return true;
    }
  }

  return false;
}

void _left_rotate(rb_node_t *node) {
  rb_node_t *right_subtree = node->right;
  node->right = right_subtree->left;
  if (right_subtree->left != NULL) {
    right_subtree->left->parent = node;
  }

  right_subtree->parent = node->parent;
  if (node->parent == NULL) {
    node->tree->root = right_subtree;
  } else if (node == node->parent->left) {
    node->parent->left = right_subtree;
  } else {
    node->parent->right = right_subtree;
  }

  right_subtree->left = node;
  node->parent = right_subtree;
}

void _right_rotate(rb_node_t *node) {
  rb_node_t *left_subtree = node->left;
  node->left = left_subtree->right;
  if (left_subtree->right != NULL) {
    left_subtree->right->parent = node;
  }

  left_subtree->parent = node->parent;
  if (node->parent == NULL) {
    node->tree->root = left_subtree;
  } else if (node == node->parent->right) {
    node->parent->right = left_subtree;
  } else {
    node->parent->left = left_subtree;
  }

  left_subtree->right = node;
  node->parent = left_subtree;
}

rb_node_t *uncle(rb_node_t *node) {
  if (left_child(node->parent)) {
    return node->parent->parent->right;
  } else if (right_child(node->parent)) {
    return node->parent->parent->left;
  } else {
    return NULL;
  }
}

void _fixup(rb_node_t *node) {
  rb_node_t *p = node;
  while (p->parent != NULL && p->parent->parent && p->parent->color == RED) {
    if (p->parent == p->parent->parent->left) {
      rb_node_t *right_uncle = p->parent->parent->right;
      if (right_uncle != NULL && right_uncle->color == RED) {
        p->parent->color = BLACK;
        right_uncle->color = BLACK;
        p->parent->parent->color =  RED;
        p = p->parent->parent;
      } else {
        if (p == p->parent->right) {
          p = p->parent;
          _left_rotate(p);
        }

        p->parent->color = BLACK;
        if (p->parent->parent != NULL) {
          p->parent->parent->color = RED;
          _right_rotate(p->parent->parent);
        }
      }
    } else if (p->parent == p->parent->parent->right) {
      rb_node_t *left_uncle = p->parent->parent->left;
      if (left_uncle != NULL && left_uncle->color == RED) {
        p->parent->color = BLACK;
        left_uncle->color = BLACK;
        p->parent->parent->color =RED;
        p = p->parent->parent;
      } else {
        if (p == p->parent->left) {
          p = p->parent;
          _right_rotate(p);
        }

        p->parent->color = BLACK;
        if (p->parent->parent != NULL) {
          p->parent->parent->color = RED;
          _left_rotate(p->parent->parent);
        }
      }
    }
  }
  node->tree->root->color = BLACK;
}

void _print_in_order(rb_node_t *node) {
  if (node->left != NULL) {
    _print_in_order(node->left);
  }
  printf("%d\n", *((int *) node->data));
  if (node->right != NULL) {
    _print_in_order(node->right);
  }
}

void print_in_order(rb_tree_t *tree) {
  rb_node_t *root = tree->root;
  _print_in_order(root);
}

rb_node_t *_find(rb_tree_t *tree, rb_node_t *node, void *value) {
  int cmp = tree->find(node->data, value);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    if (node->right != NULL) {
      return _find(tree, node->right, value);
    } else {
      return NULL;
    }
  } else {
    if (node->left != NULL) {
      return _find(tree, node->left, value);
    } else {
      return NULL;
    }
  }
}

rb_node_t *find(rb_tree_t *tree, void *value) {
  rb_node_t *root = tree->root;
  if (root == NULL) {
    return NULL;
  }
  return _find(tree, root, value);
}