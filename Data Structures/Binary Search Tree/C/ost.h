#ifndef _OST_H
#define _OST_H
#include "helper.h"

void new_Node(Node *this, Node *left, Node *right, Node *parent, int *key);
void copy_Node(Node *this, Node *other);
void tree_insert(BST *this, Node *new);
void tree_delete(BST *this, Node *z);
Node *os_select(Node *x, int i);
int os_rank(BST *this, Node *x);

#endif