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
    ComponentDef renderDef = {
        .createCallback = createRenderComponent,
        .getCallback = getRenderComponent,
        .removeCallback = removeRenderComponent
    };
    RegisterComponentDef(COMPONENT_RENDER, renderDef);

    ComponentDef shapeRenderDef = {
        .createCallback = createShapeRenderComponent,
        .getCallback = getShapeRenderComponent,
        .removeCallback = removeShapeRenderComponent
    };
    RegisterComponentDef(COMPONENT_SHAPE_RENDER, shapeRenderDef);

    return 0;
}

void DestroyRenderSystems(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning Render arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}

int createRenderComponent(void **renderComp) {
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

void UpdateRenderSystem(void) {
    for (size_t entityId = 0; entityId < AListSize(&renderEntities); ++entityId) {
        TransformComp *transfComp = GetComponent(COMPONENT_TRANSFORM, entityId);
        RenderComp *renderComp = GetComponent(COMPONENT_RENDER, entityId);

        if (transfComp == NULL || renderComp == NULL) {
            continue;
        }

        Rectangle src = renderComp->sprite.source;
        src.width = renderComp->flipX ? -src.width : src.width;
        src.height = renderComp->flipY ? -src.height : src.height;

        Rectangle dest = {transfComp->position.x, transfComp->position.y,
                          renderComp->sprite.source.width * transfComp->scale.x,
                          renderComp->sprite.source.height * transfComp->scale.y};

        Vector2 pivotCalc = Vector2Multiply(renderComp->pivot, transfComp->scale);

        DrawTexturePro(renderComp->sprite.tex, src, dest, pivotCalc,
                       transfComp->rotation, renderComp->tint);
    }
}

int createShapeRenderComponent(void **shapeRenderComp) {
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

void removeRenderComponent(int renderCompId){
    renderComponents[renderCompId].enabled = false;
}

void removeShapeRenderComponent(int shapeRenderCompId) {
    shapeRenderComponents[shapeRenderCompId].enabled = false;
}

void *getRenderComponent(int renderCompId) {
    return &renderComponents[renderCompId];
}

void *getShapeRenderComponent(int shapeRenderCompId) {
    return &shapeRenderComponents[shapeRenderCompId];
}

void addEntityToRenderSystem(int entityId) {
    AListAppend(&renderEntities, entityId);
}
