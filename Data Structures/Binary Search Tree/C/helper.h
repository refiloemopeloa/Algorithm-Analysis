#ifndef _HELPER_H
#define _HELPER_H
#include "arena.h"

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int key;
    char visited;
} Node;

typedef struct BST
{
    Node *root;
} BST;

Node *get_Node(BST *this, int key);
Node *min_Node(Node *this);
void transplant(BST *this, Node *u, Node *v);
void reset_visited(Node *this);
void generate_random_set(int *array, int *size, int *start);
void generate_random_set_r(int *array, int *size, int *start, unsigned int *seed);
void printTreeHelper(Node *node, char *prefix, int isLeft);
void printTree(BST *tree);
void inorder_tree_walk(Node *this);
Node *max_Node(BST *this);
int tree_height(Node *this);
void tree_free(Node *this);

#endif