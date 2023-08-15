#include "mem.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void ArenaInit(Arena *a, size_t capacity) {
    a->buff = malloc(capacity * sizeof(unsigned char));
    a->buffLen = capacity;
    a->currOffset = 0;
    a->prevOffset = 0;
}

static bool isPowerOfTwo(uintptr_t x) { return (x & (x - 1)) == 0; }

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

void *ArenaAllocAligned(Arena *a, size_t size, size_t align) {
    uintptr_t currPtr = (uintptr_t)a->buff + (uintptr_t)a->currOffset;
    uintptr_t offset = alignForward(currPtr, align);
    offset -= (uintptr_t)a->buff;

    if (offset + size <= a->buffLen) {
        void *ptr = &a->buff[offset];
        a->prevOffset = offset;
        a->currOffset = offset + size;
        
        memset(ptr, 0, size);
        return ptr;
    }

    assert(0 && "Out of memory in this arena");
    return NULL;
}

void *ArenaAlloc(Arena *a, size_t size) {
    return ArenaAllocAligned(a, size, DEFAULT_ALIGNMENT);
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

void *ArenaRealloc(Arena *a, void *oldMemory, size_t oldSize, size_t newSize) {
    return ArenaReallocAligned(a, oldMemory, oldSize, newSize, DEFAULT_ALIGNMENT);
}

void ArenaClear(Arena *a) {
    a->prevOffset = 0;
    a->currOffset = 0;
}

void ArenaDestroy(Arena *a) {
    ArenaClear(a);
    free(a->buff);
}

TempArena TempArenaBegin(Arena *a) {
    TempArena temp;
    temp.arena = a;
    temp.prevOffset = a->prevOffset;
    temp.currOffset = a->currOffset;
    return temp;
}

void TempArenaEnd(TempArena temp) {
    temp.arena->prevOffset = temp.prevOffset;
    temp.arena->currOffset = temp.currOffset;
}
