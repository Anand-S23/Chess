#include "memory.h"

internal memory_arena InitMemoryArena(void *memory, u32 size)
{
    memory_arena arena; 
    arena.memory = memory;
    arena.memory_size = size;
    arena.position = 0;

    return arena;
}

internal void ClearMemoryArena(memory_arena *arena)
{
    arena->position = 0;
}

internal void *AllocateMemoryArena(memory_arena *arena, u32 size)
{
    void *memory = 0;

    if(arena->position + size <= arena->memory_size)
    {
        memory = (char *)arena->memory + arena->position;
        arena->position += size;
    }

    return memory;
}