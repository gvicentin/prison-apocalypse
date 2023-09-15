#ifndef ECS_H
#define ECS_H

#include <stdbool.h>
#include "raylib.h"
#include "assets.h"
#include "utils.h"

typedef enum {
    COMP_SPRITERENDER = 0,
    COMP_ANIMRENDER,
    COMP_MAPRENDER,
    COMP_COUNT
} CompType;

typedef enum {
    SYSTEM_RENDER = 0,
    SYSTEM_ANIMATION,
    SYSTEM_MAP
} SystemType;

typedef struct Entity {
    bool enabled;
    int components[COMP_COUNT];
} Entity;

typedef struct SpriteRender {
    bool enabled;
    Sprite sprite;
    Color tint;
    Vector2 position, scale;
    bool flipX, flipY;
    float rotation;
} SpriteRender;

typedef struct AnimRender {
    bool enabled;
    Animation anim;
    float frameTime;
} AnimRender;

typedef struct MapRender {
    bool enabled;
    Map map;
    Vector2 tileSize, scale;
    Vector2 screenSize;
    int renderLayersCount;
    RenderTexture2D renderLayers[MAX_MAP_LAYERS];
} MapRender;

int EntityCompInit(void);
void EntityCompReset(void);
void EntityCompDestroy(void);

int EntityCreate(void);
void EntityRemove(int entityId);

void *ComponentCreate(int entityId, CompType type);
void ComponentRemove(int entityId, CompType type);

void SystemRenderEntities(AList *renderEntities);

void SystemAnimationUpdate(AList *animEntities, float dt);

void SystemMapInit(int mapEntity);
void SystemMapRenderLayer(int mapEntity, int layer);

#endif // !ECS_H
