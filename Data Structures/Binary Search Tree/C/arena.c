#include "arena.h"
/**
 * @file arena.h
 * @brief A simple arena-based memory allocator implementation
 */

/**
 * @brief Creates and initializes a new memory arena
 *
 * @param size The total size of the arena in bytes
 * @return Pointer to the newly created Arena, or NULL if allocation failed
 *
 * @note This function allocates memory for both the Arena structure and the
 *       underlying memory block. Use arena_destroy() to properly clean up.
 */
Arena *arena_create(unsigned long long size)
{
    Arena *arena = (Arena *)malloc(sizeof(Arena));
    if (!arena)
        return NULL;

    arena->memory = malloc(size);
    if (!arena->memory)
    {
        free(arena);
        return NULL;
    }

    arena->total_size = size;
    arena->used = 0;
    arena->first_alloc = NULL;

    return arena;
}

/**
 * @brief Allocates memory from the arena
 *
 * @param arena Pointer to the arena from which to allocate
 * @param size Number of bytes to allocate
 * @return Pointer to the allocated memory, or NULL if allocation failed
 *
 * @note The allocated size is aligned to 8 bytes for better performance.
 * @note Allocation is sequential and very fast, but individual frees are limited.
 */
void *arena_alloc(Arena *arena, unsigned long long size)
{
    if (!arena || size == 0)
        return NULL;

    // Align size to 8 bytes for better performance
    size_t aligned_size = (size + 7) & ~7;
    size_t total_needed = sizeof(AllocHeader) + aligned_size;

    if (arena->used + total_needed > arena->total_size)
    {
        fprintf(stderr, "Arena out of memory\n");
        return NULL;
    }

    // Get pointer to new allocation
    AllocHeader *header = (AllocHeader *)((uint8_t *)arena->memory + arena->used);
    header->size = aligned_size;
    header->is_free = false;
    header->next = NULL;

    // Link into allocation list
    if (!arena->first_alloc)
    {
        arena->first_alloc = header;
    }
    else
    {
        AllocHeader *curr = arena->first_alloc;
        while (curr->next)
            curr = curr->next;
        curr->next = header;
    }

    arena->used += total_needed;

    // Return pointer after header
    return (void *)(header + 1);
}

/**
 * @brief Marks a specific allocation as free
 *
 * @param arena Pointer to the arena containing the allocation
 * @param ptr Pointer to the memory to free
 *
 * @note This only marks the allocation as free but doesn't reclaim the space.
 * @note Use arena_reset() to reclaim all memory in the arena.
 * @warning The pointer must point to memory allocated from this specific arena.
 */
void arena_free(Arena *arena, void *ptr)
{
    if (!arena || !ptr)
        return;

    // Get header from user pointer
    AllocHeader *header = (AllocHeader *)ptr - 1;

    // Verify this pointer belongs to arena
    if ((void *)header < arena->memory ||
        (void *)header >= (void *)((uint8_t *)arena->memory + arena->used))
    {
        fprintf(stderr, "Invalid pointer passed to arena_free\n");
        return;
    }

    header->is_free = true;
}

/**
 * @brief Resets the arena, making all memory available for new allocations
 *
 * @param arena Pointer to the arena to reset
 *
 * @note This function is very fast as it simply resets the usage counter.
 * @warning All pointers previously allocated from this arena become invalid.
 */
void arena_reset(Arena *arena)
{
    if (!arena)
        return;
    arena->used = 0;
    arena->first_alloc = NULL;
}

/**
 * @brief Destroys the arena and frees all associated memory
 *
 * @param arena Pointer to the arena to destroy
 *
 * @note This function completely deallocates the arena and its memory block.
 * @warning After calling this function, the arena pointer becomes invalid.
 */
void arena_destroy(Arena *arena)
{
    if (!arena)
        return;
    free(arena->memory);
    free(arena);
}

/**
 * @brief Prints statistics about the arena's usage
 *
 * @param arena Pointer to the arena to analyze
 *
 * @note This function provides insights into memory usage patterns and
 *       can help identify memory leaks or inefficient usage.
 */
void arena_stats(Arena *arena)
{
    if (!arena)
        return;

    printf("Arena Statistics:\n");
    printf("  Total size: %zu bytes\n", arena->total_size);
    printf("  Used: %zu bytes\n", arena->used);
    printf("  Available: %zu bytes\n", arena->total_size - arena->used);
    printf("  Usage: %.2f%%\n", (double)arena->used / arena->total_size * 100);

    int alloc_count = 0;
    int free_count = 0;
    AllocHeader *curr = arena->first_alloc;
    while (curr)
    {
        if (curr->is_free)
            free_count++;
        else
            alloc_count++;
        curr = curr->next;
    }
    printf("  Active allocations: %d\n", alloc_count);
    printf("  Freed allocations: %d\n", free_count);
}

unsigned long long allocation;
Arena *arena;

// Example usage
#ifdef MAIN
int main()
{
    // Create arena with 1MB of memory
    unsigned long long size = 1024 * 1024 * 1024;
    size *= 8;
    Arena *arena = arena_create(size);
    if (!arena)
    {
        fprintf(stderr, "Failed to create arena\n");
        return 1;
    }

    printf("Created arena with 1GB\n\n");

    // Allocate some memory
    int *numbers = (int *)arena_alloc(arena, 1024 * 1024 * 1024 * sizeof(int));
    char *string = (char *)arena_alloc(arena, 256);
    double *floats = (double *)arena_alloc(arena, 5 * sizeof(double));

    if (numbers && string && floats)
    {
        // Use the allocations
        for (int i = 0; i < 10; i++)
            numbers[i] = i * 10;
        strcpy(string, "Hello from arena!");
        for (int i = 0; i < 5; i++)
            floats[i] = i * 1.5;

        printf("Allocated and initialized memory\n");
        printf("Numbers: %d, %d, %d...\n", numbers[0], numbers[1], numbers[2]);
        printf("String: %s\n", string);
        printf("Floats: %.1f, %.1f, %.1f...\n\n", floats[0], floats[1], floats[2]);
    }

    arena_stats(arena);
    printf("\n");

    // char ch;
    
    // while (1)
    // {
    //     ch = getchar(); // Read a character
        
    //     if (ch == '\n')
    //     {
    //         printf("Enter key pressed. Terminating program.\n");
    //         break; // Exit the loop and then the program
    //     }
    //     else
    //     {
    //         // Process other input if needed
    //         printf("You typed: %c\n", ch);
    //     }
    // }

    int a = 0;

    while (a<1000000000)
    {
        a++;
    }

    // Free some allocations
    arena_free(arena, string);
    printf("Freed string allocation\n\n");

    arena_stats(arena);
    printf("\n");

    // Reset arena (reclaim all memory)
    arena_reset(arena);
    printf("Reset arena\n\n");

    arena_stats(arena);
    
    // Clean up
    arena_destroy(arena);
    printf("\nArena destroyed\n");

    return 0;
}
#endif