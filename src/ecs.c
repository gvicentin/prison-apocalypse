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
static void *createSpriteRender(void);
static void *createAnimRender(void);
static void *createMapRender(void);

static Arena arenaAlloc;

// entities
static Entity *entities;
static int entitiesCount;

// components
static SpriteRender *compSpriteRender;
static AnimRender *compAnimRender;
static MapRender *compMapRender;

static int compCounts[COMP_COUNT];

static int compSizes[] = {sizeof(SpriteRender), sizeof(AnimRender), sizeof(MapRender)};

static void * (*compCreateFP[])(void) = {createSpriteRender, createAnimRender, createMapRender};

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
    assert(entitiesCount + 1 < MAX_ENTITIES);
    Entity *entity = &entities[entitiesCount];
    entity->enabled = true;
    for (int compId = 0; compId < COMP_COUNT; ++compId) {
        entity->components[compId] = NULL_COMP;
    }
    return entitiesCount++;
}

void EntityRemove(int entityId) {
    Entity *removeEntity = &entities[entityId];
    *removeEntity = entities[--entitiesCount];
}

static int getNextAvailableComp(CompType type) {
}

void *ComponentCreate(int entityId, CompType type) {
    int compId;
    entities[entityId].components[type] = compId;
    return NULL;
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
