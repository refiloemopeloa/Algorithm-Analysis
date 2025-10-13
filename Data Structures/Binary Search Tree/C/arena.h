#ifndef _ARENA_H
#define _ARENA_H
/**
 * @file arena_allocator.h
 * @brief A simple arena (bump) allocator implementation for efficient memory management
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * @struct Arena
 * @brief Represents a memory arena for efficient allocations
 */
typedef struct Arena {
    uint8_t *memory;      /**< Pointer to the allocated memory block */
    size_t total_size;    /**< Total size of the memory block in bytes */
    size_t offset;        /**< Current offset for the next allocation */
} Arena;

Arena* arena_create(size_t size);
void* arena_alloc(Arena *arena, size_t size);
void arena_free(Arena *arena, void *ptr);
void arena_reset(Arena *arena);
void arena_destroy(Arena *arena);

extern unsigned long long allocation;
extern Arena *arena;

#endif