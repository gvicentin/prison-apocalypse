#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

#define MAX_ANIM_FRAMES 4
#define MAX_MAP_LAYERS  3
#define MAX_MAP_WIDTH   64
#define MAX_MAP_HEIGHT  64
#define MAX_MAP_TILES   MAX_MAP_WIDTH *MAX_MAP_HEIGHT

typedef enum {
    ASSET_LOADER_SPRITESHEET = 0,
    ASSET_LOADER_ANIMATION,
    ASSET_LOADER_MAP,
    ASSET_LOADER_COUNT
} AssetLoader;

typedef enum {
    ASSET_TEXTURE = 0,
    ASSET_SPRITE,
    ASSET_ANIMATION,
    ASSET_TILE,
    ASSET_MAP,
    ASSET_COUNT
} AssetType;

typedef struct Sprite {
    Texture2D tex;
    Rectangle source;
} Sprite;

typedef struct Animation {
    int frameCount;
    float frameDuration;
    Sprite frames[MAX_ANIM_FRAMES];
} Animation;

typedef struct Tile {
    int id;
    Sprite sprite;
} Tile;

typedef struct Map {
    int width, height;
    int layersCount;
    int tiles[MAX_MAP_LAYERS][MAX_MAP_TILES];
} Map;

int AssetsInit(void);

void AssetAdd(AssetLoader loader, const char *name);

int AssetLoadSync(void);

// TODO: implement render textures
RenderTexture2D AssetCreateTexture(int width, int height);
Sprite AssetsGetSprite(const char *name);

Animation AssetsGetAnimation(const char *name);

Tile AssetsGetTile(int tileId);

Map AssetGetMap(const char *name);

void AssetsDestroy(void);

#endif // !ASSETS_H
