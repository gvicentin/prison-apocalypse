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

void DrawMap(MapRender *mapRender, Vector2 cameraPos) {
    // only using first layer for now
    int width = mapRender->map->width;
    int height = mapRender->map->height;
    int *tiles = mapRender->map->layers[0].tiles;

    int xStart = (int) (cameraPos.x / mapRender->tileSize.x) - 3;
    int xEnd = xStart + 6;
    int yStart = (int ) (cameraPos.y / mapRender->tileSize.y) - 3;
    int yEnd = yStart + 6;

    for (int y = yStart; y < yEnd; ++y) {
        for (int x = xStart; x < xEnd; ++x) {
            if (y < 0 || y >= height || x < 0 || x >= width) {
                continue;
            }

            int tile = tiles[y * width + x];
            Sprite tileSprite = mapRender->map->tileset[tile];
            Rectangle dest = {x * mapRender->tileSize.x + mapRender->offset.x,
                              y * mapRender->tileSize.y + mapRender->offset.y, mapRender->tileSize.x,
                              mapRender->tileSize.y};
            DrawTexturePro(tileSprite.tex, tileSprite.source, dest, Vector2Zero(), 0,
                           WHITE);
        }
    }
}
