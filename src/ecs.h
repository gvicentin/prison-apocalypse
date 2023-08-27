#ifndef ECS_H
#define ECS_H

#include "raylib.h"
#include "assets.h"

typedef struct SpriteRender {
    Sprite sprite;
    Vector2 position;
    Vector2 scale;
    float rotation;
    Color tint;
} SpriteRender;

typedef struct MapRender {
    Map *map;
    Vector2 offset;
    Vector2 scale;
    Vector2 tileSize;
} MapRender;

void DrawSprite(SpriteRender *spriteRender);
void DrawMap(MapRender *mapRender, Vector2 cameraPos);

#endif // !ECS_H
