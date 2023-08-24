#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

#define MAX_ANIM_FRAMES 4
#define MAX_TILESETS    32
#define MAX_MAP_LAYERS  4
#define MAX_MAP_WIDTH   64
#define MAX_MAP_HEIGHT  64

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

typedef struct MapLayer {
    int width, height;
    int tiles[MAX_MAP_WIDTH * MAX_MAP_HEIGHT];
} MapLayer;

typedef struct Map {
    int tilesetCount;
    int width, height;
    int layersCount;
    Sprite tileset[MAX_TILESETS];
    MapLayer layers[MAX_MAP_LAYERS];
} Map;

int AssetsInit(void);

void AssetAdd(AssetLoader loader, const char *name);

int AssetLoadSync(void);

Sprite AssetsGetSprite(const char *name);
Animation AssetsGetAnimation(const char *name);
Map *AssetGetMap(const char *name);

void AssetsDestroy(void);

#endif // !ASSETS_H
