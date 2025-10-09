#include "stdio.h"
#include "stdlib.h"

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int key;
    char visited;
} Node;

#define nullptr ((void *)0);

typedef struct
{
    Node *root;
} BST;

void new_Node(Node *this, Node *left, Node *right, Node *parent, int *key)
{
    this->left = left;
    this->right = right;
    this->parent = parent;
    this->key = key;
    this->visited = 0;
}

void new_BST(BST *this, int *array, int *size)
{
    ;
}
void copy_Node(Node *this, Node *other)
{
    this->left = other->left;
    this->right = other->right;
    this->parent = other->parent;
    this->key = other->key;
}

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

void tree_insert(BST *this, Node *new)
{
    Node *ptr = this->root;
    if (ptr == NULL)
    {
        this->root = (Node *)malloc(sizeof(Node));
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

void transplant(BST* this, Node* u, Node* v) {
    if (u->parent == NULL) {
        this->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

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
}

void inorder_tree_walk(Node *this)
{
    Node *ptr = this;
    while (1)
    {
        if (ptr == NULL)
        {
            printf("\n");
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
        printf("%d ", ptr->key);
        ptr->visited = 1;
        if (ptr->right != NULL && ptr->right->visited == 0)
        {
            ptr = ptr->right;
            continue;
        }
    }
}

void reset_visited(Node *this) {
    if (this != NULL) {
        reset_visited(this->left);
        this->visited = 0;
        reset_visited(this->right);
    }
}

void generate_random_set(int* array, int* size, int* start) {
    for (int i=0, j=*start; i<*size; i++, j++) {
        array[i] = j;
    }
    srand(time(NULL));
    int temp;
    int random;
    for (int i = 0; i < *size; i++) {
        random = rand() % (*start+*size);
        temp = array[i];
        array[i] = array[random];
        array[random] = temp;        
    }
}

void printTreeHelper(Node *node, char *prefix, int isLeft) {
    if (node == NULL) {
        return;
    }
    
    printf("%s", prefix);
    printf("%s", isLeft ? "├── " : "└── ");
    printf("%d\n", node->key);
    
    char *newPrefix = (char *)malloc(strlen(prefix) + 5);
    strcpy(newPrefix, prefix);
    strcat(newPrefix, isLeft ? "│   " : "    ");
    
    if (node->left != NULL || node->right != NULL) {
        if (node->right != NULL) {
            printTreeHelper(node->right, newPrefix, node->left != NULL);
        } else if (node->left != NULL) {
            printf("%s├── (null)\n", newPrefix);
        }
        
        if (node->left != NULL) {
            printTreeHelper(node->left, newPrefix, 0);
        } else if (node->right != NULL) {
            printf("%s└── (null)\n", newPrefix);
        }
    }
    
    free(newPrefix);
}

void printTree(BST *tree) {
    printf("Binary Search Tree\n===============\n");
    if (tree == NULL || tree->root == NULL) {
        printf("(empty tree)\n");
        return;
    }
    
    printf("%d\n", tree->root->key);
    
    if (tree->root->left != NULL || tree->root->right != NULL) {
        if (tree->root->right != NULL) {
            printTreeHelper(tree->root->right, "", tree->root->left != NULL);
        } else {
            printf("├── (null)\n");
        }
        
        if (tree->root->left != NULL) {
            printTreeHelper(tree->root->left, "", 0);
        } else {
            printf("└── (null)\n");
        }
    }
}

int main()
{
    int size = 12;
    int start = 0;
    int *array = (int*)calloc(size, sizeof(int));
    generate_random_set(array, &size, &start);
    BST *tree = (BST *)malloc(sizeof(BST));

    tree->root = NULL;
    Node *node;

    for (int i = 0; i < size; i++)
    {
        node = (Node *)malloc(sizeof(Node));
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
    return 0;
}