#ifndef _BST_H
#define _BST_H
#include "helper.h"

void new_Node(Node *this, Node *left, Node *right, Node *parent, int *key);
void copy_Node(Node *this, Node *other);
void tree_insert(BST *this, Node *new);
void tree_delete(BST *this, Node *z);

#endif