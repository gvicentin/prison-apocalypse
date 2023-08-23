#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

#define MAX_ANIM_FRAMES 4

typedef enum {
    ASSET_LOADER_SPRITESHEET = 0,
    ASSET_LOADER_ANIMATION,
    ASSET_LOADER_TILESET,
    ASSET_LOADER_COUNT
} AssetLoader;

typedef enum {
    ASSET_TEXTURE = 0,
    ASSET_SPRITE,
    ASSET_ANIMATION,
    ASSET_TILE,
    ASSET_TILESET,
    ASSET_MAP,
    ASSET_COUNT
} AssetType;

typedef struct Sprite {
    Texture2D tex;
    Rectangle source;
} Sprite;

typedef struct Animation {
    Sprite frames[MAX_ANIM_FRAMES];
    int frameCount;
    float frameDuration;
} Animation;

int AssetsInit(void);

void AssetAdd(AssetLoader loader, const char *name);

int AssetLoadSync(void);

Sprite AssetsGetSprite(const char *name);
Animation AssetsGetAnimation(const char *name);

void AssetsDestroy(void);

#endif // !ASSETS_H
