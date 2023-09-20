#include "render.h"

#include "assets.h"
#include "ecs.h"
#include "utils.h"
#include "raylib.h"
#include "raymath.h"
#include <assert.h>
#include <stdlib.h>

#define ARENA_BUF_LEN Kilobyte(500)

static Arena arenaAlloc;

static RenderComp *renderComponents;
static ShapeRenderComp *shapeRenderComponents;
static AnimRenderComp *animRenderComponents;
static MapRenderComp *mapRenderComponents;

static int createRenderComponent(void **renderComp);
static int createShapeRenderComponent(void **shapeRenderComp);

static void removeRenderComponent(int renderCompId);

int InitRenderSystems(void) {
    void *backingBuffer = malloc(ARENA_BUF_LEN);
    ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
    if (backingBuffer == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
        return 1;
    }

    // arena memory allocation
    renderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(RenderComp) * MAX_COMPONENT_RENDER);
    shapeRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(ShapeRenderComp) * MAX_COMPONENT_SHAPE_RENDER);
    animRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(AnimRenderComp) * MAX_COMPONENT_ANIM_RENDER);
    mapRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(MapRenderComp) * MAX_COMPONENT_MAP_RENDER);

    // register creation callbacks
    RegisterComponentCreate(COMPONENT_RENDER, createRenderComponent);

    // register remove callbacks
    RegisterComponentRemove(COMPONENT_RENDER, removeRenderComponent);

    return 0;
}

void DestroyRenderSystems(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning Asset arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}

static int createRenderComponent(void **renderComp) {
    int compId;

    for (compId = 0; compId < MAX_COMPONENT_RENDER; ++compId) {
        if (!renderComponents[compId].enabled)
            break;
    }

    assert(compId < MAX_COMPONENT_RENDER);
    if (compId >= MAX_COMPONENT_RENDER) {
        *renderComp = NULL;
        return NULL_COMPONENT;
    }

    renderComponents[compId] = (RenderComp){
        .enabled = true, .sprite = {0}, .pivot = Vector2Zero(), .tint = WHITE, .flipX = false, .flipY = false};

    *renderComp = &renderComponents[compId];
    return compId;
}

static int createShapeRenderComponent(void **shapeRenderComp) {}

static void removeRenderComponent(int renderCompId){}
