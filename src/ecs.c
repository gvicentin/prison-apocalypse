#include "ecs.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"
#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#define ARENA_BUF_LEN Kilobyte(500)

#define MAX_ENTITIES     1024
#define MAX_SPRITERENDER 512
#define MAX_ANIMRENDER   64
#define MAX_MAPRENDER    1

#define NULL_COMP -1

// Component specific functions
static int createSpriteRender(void **spriteRender);
static int createAnimRender(void **animRender);
static int createMapRender(void **mapRender);
static void removeSpriteRender(int spriteRenderId);
static void removeAnimRender(int animRenderId);
static void removeMapRender(int mapRender);

static Arena arenaAlloc;

// entities
static Entity *entities;
static int entitiesCount;

// components
static SpriteRender *compSpriteRender;
static AnimRender *compAnimRender;
static MapRender *compMapRender;

static int compCounts[COMP_COUNT];

static int (*compCreateFP[])(void **) = {createSpriteRender, createAnimRender,
                                         createMapRender};
static void (*compRemoveFP[])(int) = {removeSpriteRender, removeAnimRender, removeMapRender};

// systems
static AList familyRender;
static AList familyAnim;

int ECSInit(void) {
    void *backingBuffer = malloc(ARENA_BUF_LEN);
    ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
    if (backingBuffer == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
        return 1;
    }

    // entities
    entities = ArenaAlloc(&arenaAlloc, sizeof(Entity) * MAX_ENTITIES);
    entitiesCount = 0;

    // components
    compSpriteRender = ArenaAlloc(&arenaAlloc, sizeof(SpriteRender) * MAX_SPRITERENDER);
    compAnimRender = ArenaAlloc(&arenaAlloc, sizeof(AnimRender) * MAX_ANIMRENDER);
    compMapRender = ArenaAlloc(&arenaAlloc, sizeof(MapRender) * MAX_MAPRENDER);
    memset(compCounts, 0, sizeof(compCounts));

    // TODO: systems initialization
    AListInit(&familyRender, &arenaAlloc);
    AListInit(&familyAnim, &arenaAlloc);

    return 0;
}

void ECSReset(void) {
    // reset all entities
    entitiesCount = 0;
    for (int entityId = 0; entityId < entitiesCount; ++entityId) {
        Entity *entity = &entities[entityId];
        entity->enabled = false;
        for (int compId = 0; compId < COMP_COUNT; ++compId) {
            entity->components[compId] = NULL_COMP;
        }
    }

    // TODO: reset all components
    memset(compCounts, 0, sizeof(compCounts));

    // TODO: reset all systems
}

void ECSDestroy(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning ECS arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}

int EntityCreate(void) {
    // TODO: refactor
    assert(entitiesCount + 1 < MAX_ENTITIES);
    Entity *entity = &entities[entitiesCount];
    entity->enabled = true;
    for (int compId = 0; compId < COMP_COUNT; ++compId) {
        entity->components[compId] = NULL_COMP;
    }
    return entitiesCount++;
}

void EntityRemove(int entityId) {
    // TODO: refactor
    Entity *removeEntity = &entities[entityId];
    *removeEntity = entities[--entitiesCount];
    // remove all components
}

static int createSpriteRender(void **spriteRender) {
    int compId; 

    for (compId = 0; compId < MAX_SPRITERENDER; ++compId) {
        if (!compSpriteRender[compId].enabled) break;
    }

    assert(compId < MAX_SPRITERENDER);
    if (compId >= MAX_SPRITERENDER) {
        *spriteRender = NULL;
        return -1;
    }

    compSpriteRender[compId] = (SpriteRender) {
        .enabled = true,
        .sprite = {0},
        .tint = WHITE,
        .position = Vector2Zero(),
        .scale = Vector2One(),
        .flipX = false,
        .flipY = false,
        .rotation = 0.0f
    };

    return compId;
}

static void removeSpriteRender(int spriteRenderId) {
    compSpriteRender[spriteRenderId].enabled = false;
}

static int createAnimRender(void **animRender) {
    int compId;

    for (compId = 0; compId < MAX_ANIMRENDER; ++compId) {
        if (!compAnimRender[compId].enabled) break;
    }

    assert(compId < MAX_ANIMRENDER);
    if (compId >= MAX_ANIMRENDER) {
        *animRender = NULL;
        return -1;
    }

    compAnimRender[compId] = (AnimRender) {
        .enabled = true,
        .anim = {0},
        .frameTime = 0.0f
    };

    return compId;
}

static void removeAnimRender(int animRenderId) {
    compAnimRender[animRenderId].enabled = false;
}

static int createMapRender(void **mapRender) {
    return -1;
}

static void removeMapRender(int mapRender) {
}

void *ComponentCreate(int entityId, CompType type) {
    int compId;
    void *component = NULL;
    compId = compCreateFP[type](&component);
    entities[entityId].components[type] = compId;
    return component;
}

void InitMapRender(MapRender *mapRender) {
    Map *map = &mapRender->map;
    Vector2 tileSize = mapRender->tileSize;
    int layersCount = mapRender->renderLayersCount;

    for (int layer = 0; layer < layersCount; ++layer) {
        // create texture and enable for drawing
        mapRender->renderLayers[layer] = LoadRenderTexture(
            (int)tileSize.x * map->width, (int)tileSize.y * map->height);
        BeginTextureMode(mapRender->renderLayers[layer]);

        for (int y = 0; y < map->height; ++y) {
            for (int x = 0; x < map->width; ++x) {
                int tileId = map->tiles[layer][y * map->width + x];
                Tile tile = AssetsGetTile(tileId);

                // draw tile to texture layer
                float invY = map->height - 1 - y;
                Rectangle src = tile.sprite.source;
                Rectangle dest = {x * mapRender->tileSize.x,
                                  invY * mapRender->tileSize.y, src.width, src.height};
                src.height = -src.height;
                DrawTexturePro(tile.sprite.tex, src, dest, Vector2Zero(), 0, WHITE);
            }
        }

        EndTextureMode();
    }
}

void DrawSprite(SpriteRender *spriteRender) {
    Rectangle src = spriteRender->sprite.source;
    src.width = spriteRender->flipX ? -src.width : src.width;
    src.height = spriteRender->flipY ? -src.height : src.height;
    Rectangle dest = {spriteRender->position.x, spriteRender->position.y,
                      spriteRender->sprite.source.width * spriteRender->scale.x,
                      spriteRender->sprite.source.height * spriteRender->scale.y};
    DrawTexturePro(spriteRender->sprite.tex, src, dest, Vector2Zero(),
                   spriteRender->rotation, spriteRender->tint);
}

void DrawMapLayer(MapRender *mapRender, int layer) {
    Texture2D layerTex = mapRender->renderLayers[layer].texture;

    // draw map
    Rectangle src = {0, 0, layerTex.width, layerTex.height};
    Rectangle dest = {0, 0, layerTex.width * 2.5f, layerTex.height * 2.5f};
    DrawTexturePro(layerTex, src, dest, Vector2Zero(), 0, WHITE);
}

void UpdateAnimation(AnimRender *animRender, SpriteRender *spriteRender, float dt) {
    int frameCount = animRender->anim.frameCount;
    float frameDur = animRender->anim.frameDuration;
    int currentFrame = (int)(animRender->frameTime / frameDur) % frameCount;
    animRender->frameTime += dt;
    spriteRender->sprite = animRender->anim.frames[currentFrame];
}
