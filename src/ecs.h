#ifndef ECS_H
#define ECS_H

#include "raylib.h"
#include "assets.h"

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
    RenderTexture2D renderLayers[MAX_MAP_LAYERS];
    int renderLayersCount;
} MapRender;

void InitMapRender(MapRender *mapRender);

void DrawSprite(SpriteRender *spriteRender);
void DrawMapLayer(MapRender *mapRender, Camera2D camera, int layer);
void UpdateAnimation(AnimRender *animRender, SpriteRender *spriteRender, float dt);

#endif // !ECS_H
