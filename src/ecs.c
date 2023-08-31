#include "ecs.h"
#include "assets.h"
#include "raylib.h"
#include "raymath.h"

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

void DrawMapLayer(MapRender *mapRender, Camera2D camera, int layer) {
    Texture2D layerTex = mapRender->renderLayers[layer].texture;

    // draw map
    Rectangle src = { 0, 0, layerTex.width, layerTex.height };
    Rectangle dest = { 0, 0, layerTex.width * 2.5f, layerTex.height * 2.5f };
    DrawTexturePro(layerTex, src, dest, Vector2Zero(), 0, WHITE);
}

void UpdateAnimation(AnimRender *animRender, SpriteRender *spriteRender, float dt) {
    int frameCount = animRender->anim.frameCount;
    float frameDur = animRender->anim.frameDuration;
    int currentFrame = (int)(animRender->frameTime / frameDur) % frameCount;
    animRender->frameTime += dt;
    spriteRender->sprite = animRender->anim.frames[currentFrame];
}
