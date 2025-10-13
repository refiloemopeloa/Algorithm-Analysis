#include "arena.h"

/**
 * @brief Initialize a new memory arena with specified size
 * 
 * Allocates both the Arena structure and the underlying memory block.
 * 
 * @param size Total size of the arena in bytes
 * @return Pointer to initialized Arena, or NULL if allocation failed
 * 
 * @note The actual usable memory may be slightly less due to alignment
 */
Arena* arena_create(size_t size) {
    Arena *arena = (Arena*)malloc(sizeof(Arena));
    if (!arena) return NULL;
    
    arena->memory = (uint8_t*)malloc(size);
    if (!arena->memory) {
        free(arena);
        return NULL;
    }
    
    arena->total_size = size;
    arena->offset = 0;
    
    return arena;
}

/**
 * @brief Allocate memory from the arena
 * 
 * Uses bump allocation for O(1) performance. Memory is aligned to 16 bytes
 * for better cache performance and SIMD compatibility.
 * 
 * @param arena Pointer to the arena
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if out of memory or invalid parameters
 * 
 * @warning Allocations cannot be individually freed, use arena_reset() to reclaim all memory
 */
void* arena_alloc(Arena *arena, size_t size) {
    if (!arena || size == 0) return NULL;
    
    // Align to 16 bytes for better cache performance
    size_t aligned_size = (size + 15) & ~15;
    
    if (arena->offset + aligned_size > arena->total_size) {
        return NULL; // Out of memory
    }
    
    void *ptr = arena->memory + arena->offset;
    arena->offset += aligned_size;
    
    return ptr;
}

/**
 * @brief Free operation (no-op for arena allocator)
 * 
 * This function exists for API compatibility but does nothing.
 * Use arena_reset() to reclaim all arena memory at once.
 * 
 * @param arena Pointer to the arena (unused)
 * @param ptr Pointer to memory to free (unused)
 * 
 * @note Individual deallocation is not supported in bump allocators
 */
void arena_free(Arena *arena, void *ptr) {
    // No-op: arena uses bulk deallocation via reset
    (void)arena;
    (void)ptr;
}

/**
 * @brief Reset the arena, reclaiming all allocated memory
 * 
 * O(1) operation that resets the allocation offset to zero,
 * making all previously allocated memory available for new allocations.
 * 
 * @param arena Pointer to the arena to reset
 * 
 * @warning All pointers previously obtained from arena_alloc() become invalid
 */
void arena_reset(Arena *arena) {
    if (!arena) return;
    arena->offset = 0;
}

/**
 * @brief Destroy the arena and free all associated memory
 * 
 * Frees both the underlying memory block and the Arena structure itself.
 * 
 * @param arena Pointer to the arena to destroy
 * 
 * @note After destruction, the arena pointer is no longer valid
 */
void arena_destroy(Arena *arena) {
    if (!arena) return;
    free(arena->memory);
    free(arena);
}

unsigned long long allocation;
Arena *arena;