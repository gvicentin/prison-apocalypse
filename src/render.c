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

static AList renderEntities;

static int createRenderComponent(void **renderComp);
static int createShapeRenderComponent(void **shapeRenderComp);

static void removeRenderComponent(int renderCompId);
static void removeShapeRenderComponent(int shapeRenderCompId);

static void addEntityToRenderSystem(int entityId);

static void removeEntityFromRenderSystem(int entityId);

int InitRenderSystems(void) {
    void *backingBuffer = malloc(ARENA_BUF_LEN);
    ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
    if (backingBuffer == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
        return 1;
    }

    // make room for components
    renderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(RenderComp) * MAX_COMPONENT_RENDER);
    shapeRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(ShapeRenderComp) * MAX_COMPONENT_SHAPE_RENDER);
    animRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(AnimRenderComp) * MAX_COMPONENT_ANIM_RENDER);
    mapRenderComponents =
        ArenaAlloc(&arenaAlloc, sizeof(MapRenderComp) * MAX_COMPONENT_MAP_RENDER);

    // entities
    AListInit(&renderEntities, &arenaAlloc);
    AListExpand(&renderEntities, MAX_COMPONENT_RENDER);

    // register components callbacks
    RegisterComponentCreate(COMPONENT_RENDER, createRenderComponent);
    RegisterComponentCreate(COMPONENT_SHAPE_RENDER, createShapeRenderComponent);
    RegisterComponentRemove(COMPONENT_RENDER, removeRenderComponent);
    RegisterComponentRemove(COMPONENT_SHAPE_RENDER, removeShapeRenderComponent);

    // register system callbals
    RegisterSystemInsert(SYSTEM_RENDER, addEntityToRenderSystem);
    RegisterSystemRemove(SYSTEM_RENDER, removeEntityFromRenderSystem);

    return 0;
}

void DestroyRenderSystems(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning Render arena (%lu/%lu bytes used)",
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

static int createShapeRenderComponent(void **shapeRenderComp) {
    int compId;

    for (compId = 0; compId < MAX_COMPONENT_RENDER; ++compId) {
        if (!renderComponents[compId].enabled)
            break;
    }

    assert(compId < MAX_COMPONENT_SHAPE_RENDER);
    if (compId >= MAX_COMPONENT_SHAPE_RENDER) {
        *shapeRenderComp = NULL;
        return NULL_COMPONENT;
    }

    shapeRenderComponents[compId] = (ShapeRenderComp) {
        .enabled = true,
        .type = SHAPE_RENDER_RECT,
        .rect = {0},
        .center = {0},
        .radius = 0.0f
    };

    return compId;
}

void UpdateRenderSystem(void) {
    for (size_t entityId = 0; entityId < AListSize(renderEntities); ++entityId) {
        int transfId = GetComponentId(COMPONENT_TRANSFORM, entityId);
        int renderId = GetComponentId(COMPONENT_RENDER, entityId);

        if (transfId == NULL_COMPONENT || renderId == NULL_COMPONENT) {
            continue;
        }

        TransformComp *transf = &compTransform[compTransId];
        SpriteRender *spriteRender = &compSpriteRender[compSRId];

        Rectangle src = spriteRender->sprite.source;
        src.width = spriteRender->flipX ? -src.width : src.width;
        src.height = spriteRender->flipY ? -src.height : src.height;

        Rectangle dest = {transfComp->position.x, transfComp->position.y,
                          spriteRender->sprite.source.width * transfComp->scale.x,
                          spriteRender->sprite.source.height * transfComp->scale.y};

        Vector2 pivotCalc = Vector2Multiply(spriteRender->pivot, transfComp->scale);

        DrawTexturePro(spriteRender->sprite.tex, src, dest, pivotCalc,
                       transfComp->rotation, spriteRender->tint);
    }
}

static void removeRenderComponent(int renderCompId){
    renderComponents[renderCompId].enabled = false;
}

static void removeShapeRenderComponent(int shapeRenderCompId) {
    shapeRenderComponents[shapeRenderCompId].enabled = false;
}

static void addEntityToRenderSystem(int entityId) {
    AListAppend(&renderEntities, entityId);
}

static void removeEntityFromRenderSystem(int entityId) {
    // TODO: implement
}
