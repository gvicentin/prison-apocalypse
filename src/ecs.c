#include "ecs.h"
#include "raylib.h"
#include "raymath.h"

void DrawSprite(SpriteRender *spriteRender) {
    Rectangle dest = {spriteRender->position.x, spriteRender->position.y,
                      spriteRender->sprite.source.width * spriteRender->scale.x,
                      spriteRender->sprite.source.height * spriteRender->scale.y};
    DrawTexturePro(spriteRender->sprite.tex, spriteRender->sprite.source, dest,
                   Vector2Zero(), spriteRender->rotation, spriteRender->tint);
}

void DrawMap(MapRender *mapRender) {
    // only using first layer for now
    int width = mapRender->map->width;
    int height = mapRender->map->height;
    int *tiles = mapRender->map->layers[0].tiles;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tile = tiles[y * width + x];
            Sprite tileSprite = mapRender->map->tileset[tile];
            Vector2 tileSize = {tileSprite.source.width, tileSprite.source.height};
            tileSize = Vector2Multiply(tileSize, mapRender->scale);
            Rectangle dest = {x * tileSize.x + mapRender->offset.x,
                              y * tileSize.y + mapRender->offset.y, tileSize.x,
                              tileSize.y};
            DrawTexturePro(tileSprite.tex, tileSprite.source, dest, Vector2Zero(), 0,
                           WHITE);
        }
    }
}
