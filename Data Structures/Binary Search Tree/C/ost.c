#include "stdio.h"
#include "stdlib.h"
#include "ost.h"

/**
 * @brief Initialize a new node with given parameters
 * 
 * Sets all fields of a Node structure to the specified values.
 * Initializes rank to 0 and visited flag to 0.
 * 
 * @param this The node to initialize
 * @param left Pointer to left child node
 * @param right Pointer to right child node
 * @param parent Pointer to parent node
 * @param key Pointer to integer key value
 */
void new_Node(Node *this, Node *left, Node *right, Node *parent, int *key)
{
    this->left = left;
    this->right = right;
    this->parent = parent;
    this->key = *key;
    this->rank = 0;
    this->visited = 0;
}

/**
 * @brief Copy data from one node to another
 * 
 * Copies all data fields (except visited flag) from source node to destination node.
 * 
 * @param this Destination node to copy data to
 * @param other Source node to copy data from
 */
void copy_Node(Node *this, Node *other)
{
    this->left = other->left;
    this->right = other->right;
    this->parent = other->parent;
    this->key = other->key;
    this->rank = other->rank;
}

/**
 * @brief Insert a new node into the binary search tree with rank maintenance
 * 
 * Inserts a new node while maintaining rank information for order statistics.
 * The rank represents subtree size. Updates ranks of all ancestor nodes during insertion.
 * 
 * @param this The binary search tree to insert into
 * @param new The node to be inserted into the tree
 */
void tree_insert(BST *this, Node *new)
{
    Node *ptr = this->root;
    new->rank = 1;
    if (ptr == NULL)
    {
        this->root = (Node *)malloc(sizeof(Node));
        copy_Node(this->root, new);
        return;
    }
    while (1)
    {
        if (ptr->left == NULL && ptr->right != NULL)
            ptr->rank = ptr->right->rank + 2;
        else if (ptr->left != NULL && ptr->right == NULL)
            ptr->rank = ptr->left->rank + 2;
        else if (ptr->left == NULL && ptr->right == NULL)
            ptr->rank++;
        else
            ptr->rank = ptr->left->rank + ptr->right->rank + 2;
        if (ptr->key > new->key)
        {
            if (ptr->left == NULL)
            {
                new->parent = ptr;
                ptr->left = new;
                return;
            }
            ptr = ptr->left;
        }
        else
        {
            if (ptr->right == NULL)
            {
                new->parent = ptr;
                ptr->right = new;
                return;
            }
            ptr = ptr->right;
        }
    }
}

/**
 * @brief Delete a node from the binary search tree with rank maintenance
 * 
 * Removes a node while maintaining rank information. Decrements ranks of all
 * ancestor nodes and frees the deleted node's memory.
 * 
 * @param this The binary search tree to delete from
 * @param z The node to be deleted from the tree
 */
void tree_delete(BST *this, Node *z)
{
    Node *ptr = z->parent;
    while (ptr != NULL)
    {
        ptr->rank--;
        ptr = ptr->parent;
    }
    if (z->left == NULL)
    {
        transplant(this, z, z->right);
    }
    else if (z->right == NULL)
    {
        transplant(this, z, z->left);
    }
    else
    {
        Node *y = min_Node(z->right);
        if (y != z->right)
        {
            transplant(this, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(this, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    free(z);
}
/**
 * @brief Find the i-th smallest element in the subtree (Order Statistics Select)
 * 
 * Implements the OS-SELECT algorithm to find the node with the i-th smallest key
 * in the subtree rooted at x. Uses rank information for efficient selection.
 * 
 * @param x The root node of the subtree to search
 * @param i The rank position to find (1-based index)
 * @return Node* The node with the i-th smallest key, or NULL if not found
 */
Node *os_select(Node *x, int i)
{
    Node *this = x;
    int r;
    while (1)
    {
        if (this->left != NULL)
        {
            r = this->left->rank + 1;
        }
        else
            r = 1;
        if (i == r)
            return this;
        else if (i < r)
        {
            this = this->left;
        }
        else
        {
            this = this->right;
            i -= r;
        }
    }
}

/**
 * @brief Determine the rank of a node in the tree (Order Statistics Rank)
 * 
 * Implements the OS-RANK algorithm to find the position of node x in the
 * sorted order of all keys in the tree. Returns -1 if node is NULL.
 * 
 * @param this The binary search tree containing the node
 * @param x The node to find the rank for
 * @return int The rank of the node (1-based position), or -1 if node is NULL
 */
int os_rank(BST *this, Node *x)
{
    int r;
    if (x == NULL)
        return -1;
    if (x->left != NULL)
    {
        r = x->left->rank + 1;
    }
    else
        r = 1;
    Node *y = x;

    while (y != this->root)
    {
        if (y == y->parent->right)
        {
            if (y->parent->left != NULL)
                r += y->parent->left->rank + 1;
            else r++;
        }
        y = y->parent;
    }
    return r;
}

#ifdef MAIN
#define BRANCH 0
int main()
{
    int size = 10;
    int start = -5;
#if BRANCH == 0
    int *array = (int *)calloc(size, sizeof(int));
    generate_random_set(array, &size, &start);
#elif BRANCH == 1
    int array[] = {2, 1, 0, 7, 4, 3, 5, 6, 9, 8};
#elif BRANCH == 2
    int array[] = {3, 0, 2, 1, 4, 7, 5, 6, 9, 8};
#elif BRANCH == 3
    int array[] = {1, 0, 4, 3, 2, 6, 5, 7, 9, 8};
#elif BRANCH == 4
    int array[] = {9, 0, 7, 3, 2, 1, 5, 4, 6, 8};
#elif BRANCH == 5
    int array[] = {7, 3, 1, 0, 2, 4, 6, 5, 9, 8};
#elif BRANCH == 6
    int array[] = {1,-1,-3,-4,-5,-2,0,3,2,4};
#endif
    BST *tree = (BST *)malloc(sizeof(BST));

    tree->root = NULL;
    Node *node;

    for (int i = 0; i < size; i++)
    {
        node = (Node *)malloc(sizeof(Node));
        new_Node(node, NULL, NULL, NULL, array[i]);
        tree_insert(tree, node);
    }

    // int key = 9;
    // printf("Tree before deleting %d\n", key);
    printTree(tree);
    reset_visited(tree->root);

    // tree_delete(tree, get_Node(tree, key));
    // printf("Tree after deleting %d\n", key);
    // printTree(tree);
    // reset_visited(tree->root);

    // for (int key = 1; key <= size; key++)
    // {
    //     printf("Getting node with rank %d:\n", key);
    //     printf("Node with rank %d:\t%d\n", key, os_select(tree->root, key)->key);
    // }
    int min = min_Node(tree)->key;
    int max = max_Node(tree)->key;
    for (int key = min; key <= max; key++)
    {
        printf("Getting rank of node %d:\n", key);
        printf("Rank of node %d:\t%d\n", key, os_rank(tree, get_Node(tree, key)));
    }

    return 0;
}
#endif