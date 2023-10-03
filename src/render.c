#include "render.h"

#include "assets.h"
#include "ecs.h"
#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>

#define ARENA_BUF_LEN Kilobyte(500)

static Arena arenaAlloc;

static RenderComp *renderComponents;
static ShapeRenderComp *shapeRenderComponents;
static AnimRenderComp *animRenderComponents;
static MapRenderComp *mapRenderComponents;

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

    // register components callbacks
    ComponentDef renderDef = {.createCallback = CreateRenderComponent,
                              .getCallback = GetRenderComponent,
                              .removeCallback = RemoveRenderComponent};
    RegisterComponentDef(COMPONENT_RENDER, renderDef);

    ComponentDef shapeRenderDef = {.createCallback = CreateShapeRenderComponent,
                                   .getCallback = GetShapeRenderComponent,
                                   .removeCallback = RemoveRenderShapeComponent};
    RegisterComponentDef(COMPONENT_SHAPE_RENDER, shapeRenderDef);

    ComponentDef animRenderDef = {.createCallback = CreateAnimRenderComponent,
                                  .getCallback = GetAnimationRenderComponent,
                                  .removeCallback = RemoveAnimRenderComponent};
    RegisterComponentDef(COMPONENT_ANIMATION_RENDER, animRenderDef);

    RegisterViewComponent(VIEW_RENDER, COMPONENT_TRANSFORM);
    RegisterViewComponent(VIEW_RENDER, COMPONENT_RENDER);
    RegisterViewComponent(VIEW_ANIM_RENDER, COMPONENT_RENDER);
    RegisterViewComponent(VIEW_ANIM_RENDER, COMPONENT_ANIMATION_RENDER);

    return 0;
}

void DestroyRenderSystems(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning Render arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}

static int compareRenderOrder(const void *entityA, const void *entityB) {
    int aId = *((int *)entityA);
    int bId = *((int *)entityB);

    RenderComp *renderA = GetComponent(COMPONENT_RENDER, aId);
    RenderComp *renderB = GetComponent(COMPONENT_RENDER, bId);

    return renderA->zOrder - renderB->zOrder;
}

void UpdateRenderSystem(void) {
    int entitiesCount = 0;
    int *renderEntities = GetEntitiesFromView(VIEW_RENDER, &entitiesCount);

    // sort entities before rendering
    qsort(renderEntities, entitiesCount, sizeof(int), compareRenderOrder);

    for (int renderEntityId = 0; renderEntityId < entitiesCount; ++renderEntityId) {
        int entityId = renderEntities[renderEntityId];
        TransformComp *transfComp = GetComponent(COMPONENT_TRANSFORM, entityId);
        RenderComp *renderComp = GetComponent(COMPONENT_RENDER, entityId);

        Rectangle src = renderComp->sprite.source;
        src.width = renderComp->flipX ? -src.width : src.width;
        src.height = renderComp->flipY ? -src.height : src.height;

        Rectangle dest = {transfComp->position.x, transfComp->position.y,
                          renderComp->sprite.source.width * transfComp->scale.x,
                          renderComp->sprite.source.height * transfComp->scale.y};

        Vector2 pivotCalc = renderComp->pivot;
        pivotCalc.y = renderComp->flipY ? dest.height - pivotCalc.y : pivotCalc.y;

        Color bgColor = GREEN;
        bgColor.a = 60;
        DrawRectanglePro(dest, pivotCalc, transfComp->rotation, bgColor);
        DrawTexturePro(renderComp->sprite.tex, src, dest, pivotCalc,
                       transfComp->rotation, renderComp->tint);
        DrawCircleV(transfComp->position, 4.0f, RED);
    }
}

void UpdateAnimationSystem(float dt) {
    int entitiesCount = 0;
    int *animEntities = GetEntitiesFromView(VIEW_ANIM_RENDER, &entitiesCount);

    for (int animEntityId = 0; animEntityId < entitiesCount; ++animEntityId) {
        int entityId = animEntities[animEntityId];
        RenderComp *spriteRender = GetComponent(COMPONENT_RENDER, entityId);
        AnimRenderComp *animRender = GetComponent(COMPONENT_ANIMATION_RENDER, entityId);

        int frameCount = animRender->anim.frameCount;
        float frameDur = animRender->anim.frameDuration;
        int currentFrame = (int)(animRender->frameTime / frameDur) % frameCount;
        animRender->frameTime += dt;
        spriteRender->sprite = animRender->anim.frames[currentFrame];
    }
}

int CreateRenderComponent(void **renderComp) {
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

    renderComponents[compId] = (RenderComp){.enabled = true,
                                            .sprite = {0},
                                            .pivot = Vector2Zero(),
                                            .tint = WHITE,
                                            .flipX = false,
                                            .flipY = false,
                                            .zOrder = 1};

    *renderComp = &renderComponents[compId];
    return compId;
}

int CreateShapeRenderComponent(void **shapeRenderComp) {
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

    shapeRenderComponents[compId] = (ShapeRenderComp){.enabled = true,
                                                      .type = SHAPE_RENDER_RECT,
                                                      .rect = {0},
                                                      .center = {0},
                                                      .radius = 0.0f};

    return compId;
}

int CreateAnimRenderComponent(void **animRenderComp) {
    int compId;

    for (compId = 0; compId < MAX_COMPONENT_ANIM_RENDER; ++compId) {
        if (!animRenderComponents[compId].enabled)
            break;
    }

    assert(compId < MAX_COMPONENT_ANIM_RENDER);
    if (compId >= MAX_COMPONENT_ANIM_RENDER) {
        *animRenderComp = NULL;
        return NULL_COMPONENT;
    }

    animRenderComponents[compId] =
        (AnimRenderComp){.enabled = true, .anim = {0}, .frameTime = 0.0f};

    *animRenderComp = &animRenderComponents[compId];
    return compId;
}

void RemoveRenderComponent(int renderCompId) {
    renderComponents[renderCompId].enabled = false;
}

void RemoveRenderShapeComponent(int shapeRenderCompId) {
    shapeRenderComponents[shapeRenderCompId].enabled = false;
}

void RemoveAnimRenderComponent(int animRenderCompId) {
    animRenderComponents[animRenderCompId].enabled = false;
}

void *GetRenderComponent(int renderCompId) {
    return &renderComponents[renderCompId];
}

void *GetShapeRenderComponent(int shapeRenderCompId) {
    return &shapeRenderComponents[shapeRenderCompId];
}

void *GetAnimationRenderComponent(int animRenderCompId) {
    return &animRenderComponents[animRenderCompId];
}
