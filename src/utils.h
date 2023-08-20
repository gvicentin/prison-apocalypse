#ifndef UTILS_H
#define UTILS_H

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

typedef struct AList {
    Arena *arena;
    int *elmnts;
    size_t size;
    size_t capacity;
} AList;

typedef struct HTable {
    Arena *arna;
    char **keys;
    int *elmnts;
    size_t capacity;
    size_t size;
} HTable;

// Arena Allocator
//
void ArenaInit(Arena *arena, void *backingBuffer, size_t capacity);
void ArenaReset(Arena *arena);
void *ArenaAllocAligned(Arena *arena, size_t size, size_t align);
void *ArenaReallocAligned(Arena *arena, void *oldMemory, size_t oldSize, size_t newSize,
                          size_t align);
void *ArenaAlloc(Arena *arena, size_t size);
void *ArenaRealloc(Arena *arena, void *oldMemory, size_t oldSize, size_t newSize);

TempArena TempArenaBegin(Arena *arena);
void TempArenaEnd(TempArena temp);

// ArrayList
//
void AListInit(AList *list, Arena *arena);
void AListReset(AList *list);
void AListAppend(AList *list, int elmnt);
void AListSet(AList *list, size_t idx, int elmnt);
int AListGet(AList *list, size_t idx);
int AListRemove(AList *list, size_t idx);
int AListQuickRemove(AList *list, size_t idx);

// Hash Table
//
void HTableInit(HTable *table, Arena *arena);
void HTableReset(HTable *table);
void HTableSet(HTable *table, const char *key, int elmnt);
int HTableGet(HTable *table, const char *key);

#endif // !UTILS_H
