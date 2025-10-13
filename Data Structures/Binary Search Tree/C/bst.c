#include "stdlib.h"
#include "bst.h"

/**
* @brief Node constructor
* 
* This function acts as a Node constructor. It initializes all the members of the Node struct to the given parameters.
*
* @param this The node to be initialized. Note that the node must already be declared and if it is to be dynamically allocated, the allocation must be done before passing to this function.
* @param left The left node of this.
* @param right The right node of this.
* @param parent The parent node of this.
* @param key The key associated with this.
*
* @returns Nothing
*/
void new_Node(Node *this, Node *left, Node *right, Node *parent, int *key)
{
    this->left = left;
    this->right = right;
    this->parent = parent;
    this->key = *key;
    this->visited = 0;
}

/**
 * @brief Copy constructor
 * This function acts as a Copy constructor. It initializes all the members of the Node struct to the members of the given Node.
* @param this The node to be initialized. Note that the node must already be declared and if it is to be dynamically allocated, the allocation must be done before passing to this function.
 * @param other The node whose members will be copied from.
 * 
 * @returns Nothing
 */
void copy_Node(Node *this, Node *other)
{
    this->left = other->left;
    this->right = other->right;
    this->parent = other->parent;
    this->key = other->key;
}

/**
 * @brief Insert a new node into the binary search tree
 * 
 * Inserts a new node into the BST while maintaining the binary search tree property.
 * If the tree is empty, the new node becomes the root. Otherwise, traverses the tree
 * to find the correct position based on key comparison and inserts as a leaf node.
 * 
 * @param this The binary search tree to insert into
 * @param new The node to be inserted into the tree
 */
void tree_insert(BST *this, Node *new)
{
    Node *ptr = this->root;
    if (ptr == NULL)
    {
        #ifdef ARENA
        this->root = (Node *)arena_alloc(arena,sizeof(Node));
        #else
        this->root = (Node *)malloc(sizeof(Node));
        #endif
        copy_Node(this->root, new);
        return;
    }
    while (1)
    {
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
 * @brief Delete a node from the binary search tree
 * 
 * Removes the specified node from the BST while maintaining the binary search tree property.
 * Handles three cases: node with no left child, node with no right child, and node with both children.
 * When node has two children, it finds the successor (minimum in right subtree) and replaces the node.
 * 
 * @param this The binary search tree to delete from
 * @param z The node to be deleted from the tree
 */
void tree_delete(BST *this, Node* z)
{
    if (z->left == NULL) {
        transplant(this, z, z->right);
    } else if (z->right == NULL) {
        transplant(this, z, z->left);
    } else {
        Node* y = min_Node(z->right);
        if (y != z->right) {
            transplant(this, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(this, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    #ifdef ARENA
    arena_free(arena, z);
    #else
    free(z);
    #endif
}

#ifdef MAIN
int main()
{
    allocation = 1024 * 1024 * 1024;
    arena = arena_create(allocation);
        if (!arena)
    {
        fprintf(stderr, "Failed to create arena\n");
        return 1;
    }

    printf("Created arena with %d GB\n\n", allocation);


    int size = 12;
    int start = 0;
    int *array = (int*)arena_alloc(arena,size * sizeof(int));
    generate_random_set(array, &size, &start);
    BST *tree = (BST *)arena_alloc(arena,sizeof(BST));

    tree->root = NULL;
    Node *node;

    for (int i = 0; i < size; i++)
    {
        node = (Node *)arena_alloc(arena, sizeof(Node));
        new_Node(node, NULL, NULL, NULL, array[i]);
        tree_insert(tree, node);
    }
    
    int key = 9;
    printf("Tree before deleting %d\n", key);
    printTree(tree);
    reset_visited(tree->root);
    
    tree_delete(tree, get_Node(tree, key));
    printf("Tree after deleting %d\n", key);
    printTree(tree);
    reset_visited(tree->root);

    arena_stats(arena);
    printf("\n");

    
    arena_reset(arena);
    printf("Reset arena\n\n");
    arena_stats(arena);
    
    // Clean up
    arena_destroy(arena);
    printf("\nArena destroyed\n");

    return 0;
}
#endif