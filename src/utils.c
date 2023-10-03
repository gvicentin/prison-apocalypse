#include "utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ALIST_INITIAL_CAP  16
#define HTABLE_INITIAL_CAP 16
#define FNV_OFFSET         14695981039346656037UL
#define FNV_PRIME          1099511628211UL

void ArenaInit(Arena *arena, void *backingBuffer, size_t capacity) {
    arena->buff = (unsigned char *)backingBuffer;
    arena->buffLen = capacity;
    arena->currOffset = 0;
    arena->prevOffset = 0;
}

static bool isPowerOfTwo(uintptr_t x) {
    return (x & (x - 1)) == 0;
}

static uintptr_t alignForward(uintptr_t ptr, size_t align) {
    uintptr_t p, a, mod;

    assert(isPowerOfTwo(align));

    p = ptr;
    a = (uintptr_t)align;
    mod = p & (a - 1);

    if (mod != 0) {
        // If 'p' address is not aligned, push the address to the
        // next value which is aligned
        p += a - mod;
    }
    return p;
}

void *ArenaAllocAligned(Arena *arena, size_t size, size_t align) {
    uintptr_t currPtr = (uintptr_t)arena->buff + (uintptr_t)arena->currOffset;
    uintptr_t offset = alignForward(currPtr, align);
    offset -= (uintptr_t)arena->buff;

    if (offset + size <= arena->buffLen) {
        void *ptr = &arena->buff[offset];
        arena->prevOffset = offset;
        arena->currOffset = offset + size;

        memset(ptr, 0, size);
        return ptr;
    }

    assert(0 && "Out of memory in this arena");
    return NULL;
}

void *ArenaAlloc(Arena *arena, size_t size) {
    return ArenaAllocAligned(arena, size, DEFAULT_ALIGNMENT);
}

void *ArenaReallocAligned(Arena *a, void *oldMemory, size_t oldSize, size_t newSize,
                          size_t align) {
    unsigned char *oldMem = (unsigned char *)oldMemory;

    assert(isPowerOfTwo(align));

    if (oldMem == NULL || oldSize == 0) {
        return ArenaAllocAligned(a, newSize, DEFAULT_ALIGNMENT);
    } else if (a->buff <= oldMem && oldMem < a->buff + a->buffLen) {
        if (a->buff + a->prevOffset == oldMem) {
            // reuse the same block that was previously allocated
            a->currOffset = a->prevOffset + newSize;
            if (newSize > oldSize) {
                memset(&a->buff[a->currOffset], 0, newSize - oldSize);
            }
            return oldMem;
        } else {
            void *newMemory = ArenaAllocAligned(a, newSize, align);
            size_t copySize = oldSize < newSize ? oldSize : newSize;
            memmove(newMemory, oldMemory, copySize);
            return newMemory;
        }
    }

    assert(0 && "Out of memory in this arena");
    return NULL;
}

void *ArenaRealloc(Arena *arena, void *oldMemory, size_t oldSize, size_t newSize) {
    return ArenaReallocAligned(arena, oldMemory, oldSize, newSize, DEFAULT_ALIGNMENT);
}

void ArenaReset(Arena *arena) {
    arena->prevOffset = 0;
    arena->currOffset = 0;
}

TempArena TempArenaBegin(Arena *arena) {
    TempArena temp;
    temp.arena = arena;
    temp.prevOffset = arena->prevOffset;
    temp.currOffset = arena->currOffset;
    return temp;
}

void TempArenaEnd(TempArena temp) {
    temp.arena->prevOffset = temp.prevOffset;
    temp.arena->currOffset = temp.currOffset;
}

void AListInit(AList *list, Arena *arena) {
    list->arena = arena;
    list->elmnts = ArenaAlloc(arena, ALIST_INITIAL_CAP * sizeof(int));
    list->capacity = ALIST_INITIAL_CAP;
    list->size = 0;
}

void AListExpand(AList *list, size_t newCapacity) {
    list->elmnts = ArenaRealloc(list->arena, list->elmnts, list->capacity * sizeof(int),
                                newCapacity * sizeof(int));
}

void AListReset(AList *list) {
    list->size = 0;
}

void AListAppend(AList *list, int elmnt) {
    if (list->size >= list->capacity) {
        size_t newCapacity = list->capacity << 1;
        list->elmnts =
            ArenaRealloc(list->arena, list->elmnts, list->capacity * sizeof(int),
                         newCapacity * sizeof(int));
        list->capacity = newCapacity;
    }
    list->elmnts[list->size++] = elmnt;
}

static uint64_t hashKey(const char *key) {
    // https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

void HTableInit(HTable *table, Arena *arena) {
    table->arena = arena;
    table->entries = ArenaAlloc(arena, HTABLE_INITIAL_CAP * sizeof(HTableEntry));
    table->capacity = HTABLE_INITIAL_CAP;
    table->size = 0;
}

void HTableReset(HTable *table) {
    memset(table->entries, 0, sizeof(HTableEntry) * table->capacity);
    table->size = 0;
}

static void HTableSetEntry(Arena *arena, HTableEntry *entries, size_t capacity,
                           const char *key, int elmnt, size_t *pSize) {
    uint64_t hash = hashKey(key);
    size_t index = (hash & (capacity - 1));
    char *dupKey;

    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // replace key
            entries[index].value = elmnt;
        }
        index = (index + 1) % capacity;
    }

    // duplicate string
    if (pSize != NULL) {
        dupKey = ArenaAlloc(arena, strlen(key) + 1);
        strncpy(dupKey, key, strlen(key) + 1);
        (*pSize)++;
    }

    // adding new entry
    key = (pSize == NULL) ? key : dupKey;
    entries[index].key = key;
    entries[index].value = elmnt;
}

void HTableExpand(HTable *table, size_t newCapacity) {
    HTableEntry *newEntries =
        ArenaAlloc(table->arena, newCapacity * sizeof(HTableEntry));

    for (size_t i = 0; i < table->capacity; ++i) {
        HTableEntry entry = table->entries[i];
        if (entry.key != NULL) {
            HTableSetEntry(table->arena, newEntries, newCapacity, entry.key,
                           entry.value, NULL);
        }
    }

    table->entries = newEntries;
    table->capacity = newCapacity;
}

void HTableSet(HTable *table, const char *key, int elmnt) {
    if (table->size >= table->capacity / 2) {
        HTableExpand(table, table->capacity << 1);
    }

    return HTableSetEntry(table->arena, table->entries, table->capacity, key, elmnt,
                          &table->size);
}

int HTableGet(HTable *table, const char *key) {
    uint64_t hash = hashKey(key);
    size_t index = (hash & (table->capacity - 1));

    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            return table->entries[index].value;
        }
        index = (index + 1) % table->capacity;
    }

    return -1;
}
