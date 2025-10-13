#include "helper.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
 * @brief Get the Node object
 *
 * Get the reference to the Node object with the given key
 *
 * @param this The tree where the Node is in.
 * @param key The key of the Node being searched for
 * @return A reference to the Node with the given key.
 */
Node *get_Node(BST *this, int key)
{
    Node *ptr = this->root;

    while (1)
    {
        if (ptr == NULL)
            return NULL;
        if (ptr->key == key)
            return ptr;
        if (ptr->key > key)
        {
            ptr = ptr->left;
            continue;
        }
        ptr = ptr->right;
    }
}

/**
 * @brief Find the minimum node in a subtree
 *
 * Traverses the left children of the given node to find the minimum key value
 * in the subtree rooted at the given node.
 *
 * @param this The root node of the subtree to search
 * @return Node* Pointer to the node with minimum key value, or NULL if subtree is empty
 */
Node *min_Node(Node *this)
{
    Node *ptr = this;

    if (ptr == NULL)
        return NULL;
    while (ptr->left != NULL)
    {
        ptr = ptr->left;
    }

    return ptr;
}

/**
 * @brief Replace one subtree with another
 *
 * In transplant operation, the subtree rooted at node u is replaced by
 * the subtree rooted at node v. Node u's parent becomes node v's parent.
 *
 * @param this The binary search tree containing the nodes
 * @param u The node to be replaced
 * @param v The node that replaces u (can be NULL)
 */
void transplant(BST *this, Node *u, Node *v)
{
    if (u->parent == NULL)
    {
        this->root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if (v != NULL)
    {
        v->parent = u->parent;
    }
}

/**
 * @brief Reset the visited flag for all nodes in subtree
 *
 * Performs an in-order traversal to reset the visited flag to 0
 * for all nodes in the subtree rooted at the given node.
 *
 * @param this The root node of the subtree to reset
 */
void reset_visited(Node *this)
{
    if (this != NULL)
    {
        reset_visited(this->left);
        this->visited = 0;
        reset_visited(this->right);
    }
}

/**
 * @brief Generate a random set of integers
 *
 * Fills an array with sequential integers starting from 'start' value,
 * then randomizes their order using Fisher-Yates shuffle algorithm.
 *
 * @param array The output array to fill with random numbers
 * @param size Pointer to the size of the array
 * @param start Pointer to the starting value for the number sequence
 */
void generate_random_set(int *array, int *size, int *start)
{
    for (int i = 0, j = *start; i < *size; i++, j++)
    {
        array[i] = j;
    }
    int temp;
    int random;
    for (int i = 0; i < *size; i++)
    {
        random = rand() % (*size);
        temp = array[i];
        array[i] = array[random];
        array[random] = temp;
    }
}

#ifdef parallel
void generate_random_set_r(int *array, int *size, int *start, unsigned int *seed)
{
    // Fill array with sequential values starting from 'start'
    for (int i = 0, j = *start; i < *size; i++, j++)
    {
        array[i] = j;
    }
    
    // Fisher-Yates shuffle using thread-safe rand_r
    int temp;
    int random;
    for (int i = 0; i < *size; i++)
    {
        // Generate random index within valid range [0, size-1]
        random = rand_r(seed) % (*size);
        
        // Swap elements
        temp = array[i];
        array[i] = array[random];
        array[random] = temp;
    }
}
#endif

/**
 * @brief Helper function for printing tree structure
 *
 * Recursively prints the binary tree in a horizontal tree format
 * using ASCII characters for branches. Right subtree is printed first.
 *
 * @param node The current node being printed
 * @param prefix String prefix for indentation at current level
 * @param isLeft Boolean indicating if current node is left child of its parent
 */
void printTreeHelper(Node *node, char *prefix, int isLeft)
{
    if (node == NULL)
    {
        return;
    }

    printf("%s", prefix);
    printf("%s", isLeft ? "├── " : "└── ");
    printf("%d\n", node->key);

    char *newPrefix = (char *)malloc(strlen(prefix) + 5);
    strcpy(newPrefix, prefix);
    strcat(newPrefix, isLeft ? "│   " : "    ");

    if (node->left != NULL || node->right != NULL)
    {
        if (node->right != NULL)
        {
            printTreeHelper(node->right, newPrefix, node->left != NULL);
        }
        else if (node->left != NULL)
        {
            printf("%s├── (null)\n", newPrefix);
        }

        if (node->left != NULL)
        {
            printTreeHelper(node->left, newPrefix, 0);
        }
        else if (node->right != NULL)
        {
            printf("%s└── (null)\n", newPrefix);
        }
    }

    free(newPrefix);
}

/**
 * @brief Print the entire binary search tree
 *
 * Displays the binary search tree in a hierarchical format showing
 * the tree structure with root at top and children below.
 *
 * @param tree The binary search tree to print
 */
void printTree(BST *tree)
{
    printf("Binary Search Tree\n===============\n");
    if (tree == NULL || tree->root == NULL)
    {
        printf("(empty tree)\n");
        return;
    }

    printf("%d\n", tree->root->key);

    if (tree->root->left != NULL || tree->root->right != NULL)
    {
        if (tree->root->right != NULL)
        {
            printTreeHelper(tree->root->right, "", tree->root->left != NULL);
        }
        else
        {
            printf("├── (null)\n");
        }

        if (tree->root->left != NULL)
        {
            printTreeHelper(tree->root->left, "", 0);
        }
        else
        {
            printf("└── (null)\n");
        }
    }
}

/**
 * @brief Perform iterative in-order tree traversal
 *
 * Traverses the binary search tree in-order (left, root, right) without recursion
 * using an iterative approach with a visited flag. This method prints the keys
 * in ascending order for a BST. The traversal uses parent pointers to navigate
 * back up the tree and marks nodes as visited to avoid reprocessing.
 *
 * @param this The root node of the subtree to traverse
 */
void inorder_tree_walk(Node *this)
{
    Node *ptr = this;
    while (1)
    {
        if (ptr == NULL)
        {
#ifdef PRINT
            printf("\n");
#endif
            return;
        }
        if (ptr->visited == 1)
        {
            ptr = ptr->parent;
            continue;
        }
        if (ptr->left != NULL && ptr->left->visited == 0)
        {
            ptr = ptr->left;
            continue;
        }
#ifdef PRINT
        printf("%d ", ptr->key);
#endif
        ptr->visited = 1;
        if (ptr->right != NULL && ptr->right->visited == 0)
        {
            ptr = ptr->right;
            continue;
        }
    }
}

/**
 * @brief Find the maximum node in the BST
 *
 * Traverses the right children starting from the root to find the node
 * with the maximum key value in the entire binary search tree.
 *
 * @param this The binary search tree to search
 * @return Node* Pointer to the node with maximum key value
 * @note Assumes the tree is non-empty. Caller should check for empty tree.
 */
Node *max_Node(BST *this)
{
    Node *ptr = this->root;
    while (ptr->right != NULL)
    {
        ptr = ptr->right;
    }

    return ptr;
}

/**
 * @brief Calculate the height of a binary tree/subtree
 *
 * Recursively computes the height of the binary tree rooted at the given node.
 * The height is defined as the number of edges on the longest path from the root to a leaf.
 * An empty tree has height 0.
 *
 * @param this The root node of the tree/subtree to measure
 * @return int The height of the tree (0 for empty tree)
 */
int tree_height(Node *this)
{
    int height = -1;
    if (this == NULL)
        return -1;
    int height_left = tree_height(this->left);
    if (height_left > height)
        height = height_left;
    int height_right = tree_height(this->right);
    if (height_right > height)
        height = height_right;
    height++;
    return height;
}

/**
 * @brief Recursively free all nodes in a binary tree
 *
 * Performs a post-order traversal to safely deallocate all nodes in the tree
 * rooted at the given node. Frees left subtree, right subtree, then the current node.
 *
 * @param this The root node of the tree to free (can be NULL)
 */
void tree_free(Node *this)
{
    if (this == NULL)
    {
        return;
    }
    tree_free(this->left);
    tree_free(this->right);
    arena_free(arena, this);
}