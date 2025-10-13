#include "stdio.h"
#include "stdlib.h"
#ifdef BST_TEST
#include "bst.h"
#else
#include "ost.h"
#endif
#include "math.h"
#include "unistd.h"
#include "time.h"
#ifdef parallel
#include "omp.h"
#endif

#ifdef BST_TEST
void bst_tests()
{
#define test_limit 8
#define test_base 10
#define repeat 10
    int *array;
    int size;
    int start;

    double data_insert_time[test_limit][repeat] = {0};
    double data_inorder_time[test_limit][repeat] = {0};
    double data_delete_time[test_limit][repeat] = {0};
    int data_height[test_limit][repeat] = {0};
    clock_t t;

    FILE *insert, *inorder, *delete, *height;

    insert = fopen("data/bst_insert.csv", "w");
    inorder = fopen("data/bst_inorder.csv", "w");
    delete = fopen("data/bst_delete.csv", "w");
    height = fopen("data/bst_height.csv", "w");

    for (int i = 1; i <= test_limit; i++)
    {
        if (insert == NULL || inorder == NULL || delete == NULL || height == NULL)
        {
            fprintf(stderr, "\nError opened file\n");
            exit(1);
        }
        size = pow(test_base, i);
        printf("========== RUNNING TEST FOR SIZE %d ==========\n", size);
        fprintf(insert, "%d,", size);
        fprintf(inorder, "%d,", size);
        fprintf(delete, "%d,", size);
        fprintf(height, "%d,", size);
        for (int j = 0; j < repeat; j++) // repeat x times to ensure expected result
        {
            printf("---------- Test %d.%d ----------\n", i, j);
            printf("Updating seed...\n");
            sleep(1);
            srand(time(NULL));
            start = -(rand() % size);
            printf("Generating list of random numbers of length %d...\n", size);
            array = (int *)calloc(size, sizeof(int));
            generate_random_set(array, &size, &start);
            BST *tree = (BST *)malloc( sizeof(BST));
            tree->root = NULL;
            Node *node;

            // time this
            printf("Constructing BST...\n");
            t = clock();
            for (int k = 0; k < size; k++)
            {
                node = (Node *)malloc(sizeof(Node));
                new_Node(node, NULL, NULL, NULL, &array[k]);
                tree_insert(tree, node);
            }
            t = clock() - t;
            data_insert_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            // collect this
            printf("Gettting height of BST...\n");
            data_height[i - 1][j] = tree_height(tree->root);

            printf("Inorder tree walk...\n");
            reset_visited(tree->root);
            // time this
            t = clock();
            inorder_tree_walk(tree->root);
            t = clock() - t;
            data_inorder_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            printf("Freeing memory...\n");

            // tree free
            t = clock();
            while (tree->root != NULL) {
                tree_delete(tree, tree->root);
            }
            t = clock() - t;

            data_delete_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;
            free(array);

            printf("Printing to files...\n");
            fprintf(insert, "%.15f,", data_insert_time[i - 1][j]);
            fprintf(inorder, "%.15f,", data_inorder_time[i - 1][j]);
            fprintf(delete, "%.15f,", data_delete_time[i - 1][j]);
            fprintf(height, "%d,", data_height[i - 1][j]);
            fclose(insert);
            fclose(inorder);
            fclose(delete);
            fclose(height);
            
            insert = fopen("data/bst_insert.csv", "a");
            inorder = fopen("data/bst_inorder.csv", "a");
            delete = fopen("data/bst_delete.csv", "a");
            height = fopen("data/bst_height.csv", "a");
        }
        fprintf(insert, "\n");
        fprintf(inorder, "\n");
        fprintf(delete, "\n");
        fprintf(height, "\n");

        fclose(insert);
        fclose(inorder);
        fclose(delete);
        fclose(height);

        insert = fopen("data/bst_insert.csv", "a");
        inorder = fopen("data/bst_inorder.csv", "a");
        delete = fopen("data/bst_delete.csv", "a");
        height = fopen("data/bst_height.csv", "a");
    }

    return;
}
#else
void ost_tests()
{
#define test_limit 8
#define test_base 10
#define repeat 10
    int *array;
    int size;
    int start;

    double data_insert_time[test_limit][repeat] = {0};
    double data_select_time[test_limit][repeat][repeat] = {0};
    double data_rank_time[test_limit][repeat][repeat] = {0};
    double data_delete_time[test_limit][repeat] = {0};
    int random;
    Node* random_node;
    clock_t t;

    FILE *insert, *select, *delete, *rank;

    insert = fopen("data/ost_insert.csv", "w");
    select = fopen("data/ost_select.csv", "w");
    delete = fopen("data/ost_delete.csv", "w");
    rank = fopen("data/ost_rank.csv", "w");

    for (int i = 1; i <= test_limit; i++)
    {
        if (insert == NULL || select == NULL || delete == NULL || rank == NULL)
        {
            fprintf(stderr, "\nError opened file\n");
            exit(1);
        }
        size = pow(test_base, i);
        printf("========== RUNNING TEST FOR SIZE %d ==========\n", size);
        fprintf(insert, "%d,", size);
        fprintf(select, "%d,", size);
        fprintf(delete, "%d,", size);
        fprintf(rank, "%d,", size);
        for (int j = 0; j < repeat; j++) // repeat x times to ensure expected result
        {
            printf("---------- Test %d.%d ----------\n", i, j);
            printf("Updating seed...\n");
            sleep(1);
            srand(time(NULL));
            start = -(rand() % size);
            printf("Generating list of random numbers of length %d...\n", size);
            array = (int *)calloc(size, sizeof(int));
            generate_random_set(array, &size, &start);
            BST *tree = (BST *)malloc( sizeof(BST));
            tree->root = NULL;
            Node *node;

            // time this
            printf("Constructing BST...\n");
            t = clock();
            for (int k = 0; k < size; k++)
            {
                node = (Node *)malloc(sizeof(Node));
                new_Node(node, NULL, NULL, NULL, &array[k]);
                tree_insert(tree, node);
            }
            t = clock() - t;
            data_insert_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            reset_visited(tree->root);

            printf("OS Select...\n");
            // time this
            for (int k = 0; k < repeat; k++) {
                random = (rand() % (size)) + 1;
                t = clock();
                os_select(tree->root, random);
                t = clock() - t;
                data_select_time[i - 1][j][k] = ((double)t) / CLOCKS_PER_SEC;
                fprintf(select, "%.15f,", data_select_time[i - 1][j][k]);
            }

            printf("OS Rank...\n");
            // time this
            for (int k = 0; k < repeat; k++) {
                random = start + (rand() % (size));
                random_node = get_Node(tree, random);
                t = clock();
                os_rank(tree, random_node);
                t = clock() - t;
                data_rank_time[i - 1][j][k] = ((double)t) / CLOCKS_PER_SEC;
                fprintf(rank, "%.15f,", data_rank_time[i - 1][j][k]);
            }

            printf("Freeing memory...\n");

            // tree free
            t = clock();
            while (tree->root != NULL) {
                tree_delete(tree, tree->root);
            }
            t = clock() - t;

            data_delete_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;
            free(array);

            printf("Printing to files...\n");
            fprintf(insert, "%.15f,", data_insert_time[i - 1][j]);
            fprintf(delete, "%.15f,", data_delete_time[i - 1][j]);
            fclose(insert);
            fclose(select);
            fclose(delete);
            fclose(rank);
            
            insert = fopen("data/ost_insert.csv", "a");
            select = fopen("data/ost_select.csv", "a");
            delete = fopen("data/ost_delete.csv", "a");
            rank = fopen("data/ost_rank.csv", "a");
        }
        fprintf(insert, "\n");
        fprintf(select, "\n");
        fprintf(delete, "\n");
        fprintf(rank, "\n");

        fclose(insert);
        fclose(select);
        fclose(delete);
        fclose(rank);

        insert = fopen("data/ost_insert.csv", "a");
        select = fopen("data/ost_select.csv", "a");
        delete = fopen("data/ost_delete.csv", "a");
        rank = fopen("data/ost_rank.csv", "a");
    }

    return;
}
#endif

#ifdef parallel
void bst_tests()
{
#define test_limit 7
#define test_base 10
#define repeat 10

    double data_insert_time[test_limit][repeat] = {0};
    double data_inorder_time[test_limit][repeat] = {0};
    double data_delete_time[test_limit][repeat] = {0};
    int data_height[test_limit][repeat] = {0};
    omp_set_num_threads(16);

// Parallelize outer loop - each size test runs independently
#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i <= test_limit; i++)
    {
        int size = pow(test_base, i);
        printf("========== RUNNING TEST FOR SIZE %d ==========\n", size);

// Parallelize inner loop - each repeat runs independently
#pragma omp parallel for schedule(dynamic)
        for (int j = 0; j < repeat; j++)
        {
            printf("---------- Test %d.%d ----------\n", i, j);

            // Each thread needs its own random seed
            unsigned int seed = time(NULL) ^ (omp_get_thread_num() << 16);
            int start = -(rand_r(&seed) % size);

            printf("Generating list of random numbers of length %d...\n", size);
            int *array = (int *)calloc(size, sizeof(int));
            generate_random_set_r(array, &size, &start, &seed);

            BST *tree = (BST *)malloc(sizeof(BST));
            tree->root = NULL;
            Node *node;

            // Time insertion
            printf("Constructing BST...\n");
            clock_t t = clock();
            for (int k = 0; k < size; k++)
            {
                node = (Node *)malloc(sizeof(Node));
                new_Node(node, NULL, NULL, NULL, array[k]);
                tree_insert(tree, node);
            }
            t = clock() - t;
            data_insert_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            // Collect height
            printf("Getting height of BST...\n");
            data_height[i - 1][j] = tree_height(tree->root);

            printf("Inorder tree walk...\n");
            reset_visited(tree->root);

            // Time inorder walk
            t = clock();
            inorder_tree_walk(tree->root);
            t = clock() - t;
            data_inorder_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            printf("Freeing memory...\n");

            // Time deletion
            t = clock();
            tree_free(tree->root);
            t = clock() - t;
            data_delete_time[i - 1][j] = ((double)t) / CLOCKS_PER_SEC;

            free(array);
        }
    }

    // Write results sequentially after all parallel work is done
    FILE *insert = fopen("data/bst_insert.csv", "w");
    FILE *inorder = fopen("data/bst_inorder.csv", "w");
    FILE *delete = fopen("data/bst_delete.csv", "w");
    FILE *height = fopen("data/bst_height.csv", "w");

    if (insert == NULL || inorder == NULL || delete == NULL || height == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

    for (int i = 1; i <= test_limit; i++)
    {
        int size = pow(test_base, i);

        // Write insert times
        fprintf(insert, "%d,", size);
        for (int j = 0; j < repeat - 1; j++)
        {
            fprintf(insert, "%.15f,", data_insert_time[i - 1][j]);
        }
        fprintf(insert, "%.15f\n", data_insert_time[i - 1][repeat - 1]);

        // Write inorder times
        fprintf(inorder, "%d,", size);
        for (int j = 0; j < repeat - 1; j++)
        {
            fprintf(inorder, "%.15f,", data_inorder_time[i - 1][j]);
        }
        fprintf(inorder, "%.15f\n", data_inorder_time[i - 1][repeat - 1]);

        // Write delete times
        fprintf(delete, "%d,", size);
        for (int j = 0; j < repeat - 1; j++)
        {
            fprintf(delete, "%.15f,", data_delete_time[i - 1][j]);
        }
        fprintf(delete, "%.15f\n", data_delete_time[i - 1][repeat - 1]);

        // Write heights
        fprintf(height, "%d,", (int)pow(test_base, i + 1));
        for (int j = 0; j < repeat - 1; j++)
        {
            fprintf(height, "%d,", data_height[i - 1][j]);
        }
        fprintf(height, "%d\n", data_height[i - 1][repeat - 1]);
    }

    fclose(insert);
    fclose(inorder);
    fclose(delete);
    fclose(height);

    return;
}
#endif

int main()
{
    #ifdef BST_TEST
    bst_tests();
    #else
    ost_tests();
    #endif
    return 0;
}