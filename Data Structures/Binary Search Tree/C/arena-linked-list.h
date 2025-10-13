/**
 * @file arena.h
 * @brief A simple arena-based memory allocator implementation
 */

#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Header for tracking individual allocations within the arena
 */
typedef struct AllocHeader {
    size_t size;            /**< Size of the allocated block (including alignment) */
    bool is_free;           /**< Flag indicating if this allocation is free */
    struct AllocHeader *next; /**< Pointer to the next allocation header in the list */
} AllocHeader;

/**
 * @brief Memory arena structure that manages a contiguous block of memory
 */
typedef struct {
    void *memory;           /**< Pointer to the start of the arena's memory block */
    size_t total_size;      /**< Total size of the arena in bytes */
    size_t used;            /**< Number of bytes currently used in the arena */
    AllocHeader *first_alloc; /**< Pointer to the first allocation header in the linked list */
} Arena;

/**
 * @brief Creates and initializes a new memory arena
 */
Arena* arena_create(unsigned long long size);

/**
 * @brief Allocates memory from the arena
 */
void* arena_alloc(Arena *arena, unsigned long long size);

/**
 * @brief Marks a specific allocation as free
 */
void arena_free(Arena *arena, void *ptr);

/**
 * @brief Resets the arena, making all memory available for new allocations
 */
void arena_reset(Arena *arena);

/**
 * @brief Destroys the arena and frees all associated memory
 */
void arena_destroy(Arena *arena);

/**
 * @brief Prints statistics about the arena's usage
 */
void arena_stats(Arena *arena);

extern unsigned long long allocation;
extern Arena *arena;

#endif /* ARENA_ALLOCATOR_H */