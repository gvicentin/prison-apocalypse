#ifndef MEM_H
#define MEM_H

#include <stddef.h>

#define Kilobyte(k) (k * 1024)
#define Megabyte(m) (m * 1024 * 1024)
#define Gigabyte(g) (g * 1024 * 1024 * 1024)

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct Arena {
    unsigned char *buff;
    size_t buffLen;
    size_t prevOffset;
    size_t currOffset;
} Arena;

typedef struct TempArena {
    Arena *arena;
    size_t prevOffset;
    size_t currOffset;
} TempArena;

void ArenaInit(Arena *a, size_t capacity);
void *ArenaAllocAligned(Arena *a, size_t size, size_t align);
void *ArenaAlloc(Arena *a, size_t size);
void *ArenaReallocAligned(Arena *a, void *oldMemory, size_t oldSize, size_t newSize,
                          size_t align);
void *ArenaRealloc(Arena *a, void *oldMemory, size_t oldSize, size_t newSize);
void ArenaClear(Arena *a);
void ArenaDestroy(Arena *a);

TempArena TempArenaBegin(Arena *a);
void TempArenaEnd(TempArena temp);

#endif // !MEM_H
