#ifndef ECS_H
#define ECS_H

#include "raylib.h"
#include "assets.h"

typedef enum {
    COMP_SPRITERENDER = 0,
    COMP_ANIMRENDER,
    COMP_MAPRENDER,
    COMP_COUNT
} CompType;

typedef struct Entity {
    bool enabled;
    int components[COMP_COUNT];
} Entity;

typedef struct SpriteRender {
    Sprite sprite;
    Vector2 position, scale;
    bool flipX, flipY;
    float rotation;
    Color tint;
} SpriteRender;

typedef struct AnimRender {
    Animation anim;
    float frameTime;
} AnimRender;

typedef struct MapRender {
    Map map;
    Vector2 tileSize;
    Vector2 screenSize;
    int renderLayersCount;
    RenderTexture2D renderLayers[MAX_MAP_LAYERS];
} MapRender;

int ECSInit(void);
void ECSDestroy(void);

int EntityCreate(void);
void EntityRemove(int entityId);

void *ComponentCreate(int entityId, CompType type);

void InitMapRender(MapRender *mapRender);

void DrawSprite(SpriteRender *spriteRender);
void DrawMapLayer(MapRender *mapRender, int layer);
void UpdateAnimation(AnimRender *animRender, SpriteRender *spriteRender, float dt);

#endif // !ECS_H
