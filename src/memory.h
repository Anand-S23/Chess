#ifndef MEMORY_H 
#define MEMORY_H

typedef struct memory_arena
{
    void *memory;
    u32 memory_size;
    u32 position;
} memory_arena;

internal memory_arena InitMemoryArena(void *memory, u32 size);
internal void ClearMemoryArena(memory_arena *arena);
internal void *AllocateMemoryArena(memory_arena *arena, u32 size);

#endif