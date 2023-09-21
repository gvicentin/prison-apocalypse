#include "assets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define __USE_POSIX
#include <string.h>

#include "raylib.h"
#include "utils.h"

#ifndef ASSETS_PATH
#define ASSETS_PATH "./assets"
#endif

#define ARENA_BUF_LEN Kilobyte(500)

#define ASSET_NAME_MAX 64

#define TABLE_INITIAL_SIZE 512

#define MAX_ASSETENTRIES 16
#define MAX_TEXTURES     8
#define MAX_SPRITES      256
#define MAX_ANIMATIONS   16
#define MAX_TILES        128
#define MAX_MAPS         1

typedef struct AssetEntry {
    AssetLoader loader;
    char name[ASSET_NAME_MAX];
} AssetEntry;

// Asset loader functions
static int loadSpritesheet(const char *name);
static int loadAnimation(const char *name);
static int loadMap(const char *name);

// Liner allocator
static Arena arenaAlloc;

// Asset entries to be loaded
static AssetEntry *assetEntries;
static int assetEntriesCount;

// Table for storing asset indices
static HTable assetTable;

// Pointer for all assets loaded
static Texture2D *assetTextures;
static Sprite *assetSprites;
static Animation *assetAnims;
static Tile *assetTiles;
static Map *assetMaps;

// Count of every asset type
static int assetCounts[ASSET_COUNT];

// Loaders func pointers
static int (*loaders[])(const char *) = {loadSpritesheet, loadAnimation, loadMap};

int AssetsInit(void) {
    // initialize linear allocator
    void *backingBuffer = malloc(ARENA_BUF_LEN);
    ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
    if (backingBuffer == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
        return 1;
    }

    // make room for asset entries
    assetEntries = ArenaAlloc(&arenaAlloc, sizeof(AssetEntry) * MAX_ASSETENTRIES);
    assetEntriesCount = 0;

    // make room for assets
    assetTextures = ArenaAlloc(&arenaAlloc, sizeof(Texture2D) * MAX_TEXTURES);
    assetSprites = ArenaAlloc(&arenaAlloc, sizeof(Sprite) * MAX_SPRITES);
    assetAnims = ArenaAlloc(&arenaAlloc, sizeof(Animation) * MAX_ANIMATIONS);
    assetTiles = ArenaAlloc(&arenaAlloc, sizeof(Tile) * MAX_TILES);
    assetMaps = ArenaAlloc(&arenaAlloc, sizeof(Map) * MAX_MAPS);
    memset(assetCounts, 0, sizeof(assetCounts));

    // init asset table
    HTableInit(&assetTable, &arenaAlloc);
    HTableExpand(&assetTable, 1024);

    return 0;
}

void AssetAdd(AssetLoader loader, const char *name) {
    assert(assetEntriesCount + 1 < MAX_ASSETENTRIES);
    AssetEntry *entry = &assetEntries[assetEntriesCount];
    entry->loader = loader;
    strncpy(entry->name, name, 64);
    ++assetEntriesCount;
}

static int loadSpritesheet(const char *name) {
    char imageFilepath[ASSET_NAME_MAX];
    char metaFilepath[ASSET_NAME_MAX];

    // If this fails, needs to increase maxs
    assert(assetCounts[ASSET_TEXTURE] < MAX_TEXTURES);
    assert(assetCounts[ASSET_SPRITE] < MAX_SPRITES);

    snprintf(imageFilepath, ASSET_NAME_MAX, "%s.png", name);
    snprintf(metaFilepath, ASSET_NAME_MAX, "%s.sprite", name);

    // load image from disc to GPU
    int textureCount = assetCounts[ASSET_TEXTURE];
    assetTextures[textureCount] = LoadTexture(imageFilepath);
    if (assetTextures[textureCount].id <= 0) {
        // failed to load texture
        return 1;
    }
    ++assetCounts[ASSET_TEXTURE];

    char *metaContent = LoadFileText(metaFilepath);
    if (metaContent == NULL) {
        // failed to load file
        return 1;
    }

    char *lineToken = strtok(metaContent, "\n");
    while (lineToken != NULL) {
        char sprite[64];
        int x, y, width, height;
        sscanf(lineToken, "%63s %d %d %d %d", sprite, &x, &y, &width, &height);

        // creating sprite and adding it to table
        int spriteCount = assetCounts[ASSET_SPRITE];
        assetSprites[spriteCount] =
            (Sprite){assetTextures[textureCount], {x, y, width, height}};
        HTableSet(&assetTable, sprite, spriteCount);
        ++assetCounts[ASSET_SPRITE];

        lineToken = strtok(NULL, "\n");
    }

    // cleanup
    free(metaContent);
    return 0;
}

static int loadAnimation(const char *name) {
    char animFilepath[ASSET_NAME_MAX];

    // If this fails, needs to increase max
    assert(assetCounts[ASSET_ANIMATION] < MAX_ANIMATIONS);

    snprintf(animFilepath, ASSET_NAME_MAX, "%s.anim", name);

    char *animContent = LoadFileText(animFilepath);
    if (animContent == NULL) {
        // failed to load file
        return 1;
    }

    char *lineToken = strtok(animContent, "\n");
    while (lineToken != NULL) {
        char animName[64];
        char spriteName[128];
        int frameCount;
        float frameDuration;

        sscanf(lineToken, "%31s %d %f", animName, &frameCount, &frameDuration);

        int animCount = assetCounts[ASSET_ANIMATION];
        assetAnims[animCount].frameCount = frameCount;
        assetAnims[animCount].frameDuration = frameDuration;
        for (int i = 0; i < frameCount; ++i) {
            snprintf(spriteName, 128, "%s_%d", animName, i);
            assetAnims[animCount].frames[i] = AssetsGetSprite(spriteName);
        }

        // add asset to table
        HTableSet(&assetTable, animName, animCount);
        ++assetCounts[ASSET_ANIMATION];

        lineToken = strtok(NULL, "\n");
    }

    // cleanup
    free(animContent);
    return 0;
}

static int loadMap(const char *name) {
    char mapFilepath[ASSET_NAME_MAX];
    int tilesetCount;
    int layersCount;
    int width, height;

    // If this fails, needs to increase max
    assert(assetCounts[ASSET_MAP] < MAX_MAPS);

    snprintf(mapFilepath, ASSET_NAME_MAX, "%s.map", name);

    char *mapContent = LoadFileText(mapFilepath);
    if (mapContent == NULL) {
        // failed to load file
        return 1;
    }

    char *endLine;
    char *lineToken = strtok_r(mapContent, "\n", &endLine);
    if (lineToken == NULL) {
        // should have at least one line
        return 1;
    }

    // read metadata line
    sscanf(lineToken, "%d %d %d %d", &tilesetCount, &layersCount, &width, &height);
    lineToken = strtok_r(NULL, "\n", &endLine);

    int mapCount = assetCounts[ASSET_MAP];
    assetMaps[mapCount].width = width;
    assetMaps[mapCount].height = height;
    assetMaps[mapCount].layersCount = layersCount;

    int prevTilesCount = assetCounts[ASSET_TILE];

    for (int i = 0; i < tilesetCount; ++i) {
        char spriteName[64];
        sscanf(lineToken, "%63s", spriteName);

        int tileCount = assetCounts[ASSET_TILE];
        assert(tileCount < MAX_TILES);
        assetTiles[tileCount].id = tileCount;
        assetTiles[tileCount].sprite = AssetsGetSprite(spriteName);
        ++assetCounts[ASSET_TILE];

        lineToken = strtok_r(NULL, "\n", &endLine);
    }

    for (int layer = 0; layer < layersCount; ++layer) {
        int tileAccum = 0;
        while (lineToken != NULL) {
            char *endColumn;
            char *columnToken = strtok_r(lineToken, " ", &endColumn);
            while (columnToken != NULL) {
                int tile;
                sscanf(columnToken, "%d", &tile);
                assetMaps[mapCount].tiles[layer][tileAccum++] = tile + prevTilesCount;
                columnToken = strtok_r(NULL, " ", &endColumn);
            }
            lineToken = strtok_r(NULL, "\n", &endLine);
        }
    }

    HTableSet(&assetTable, "prison", mapCount);
    ++assetCounts[ASSET_MAP];

    // cleanup
    free(mapContent);
    return 0;
}

int AssetLoadSync(void) {
    // Set correct directory to start loading
    ChangeDirectory(ASSETS_PATH);

    for (int i = 0; i < assetEntriesCount; ++i) {
        AssetLoader loader = assetEntries[i].loader;
        int err = loaders[loader](assetEntries[i].name);
        if (err != 0) {
            TraceLog(LOG_ERROR, "Error loading %s", assetEntries[i].name);
            return 1;
        }
    }

    return 0;
}

Sprite AssetsGetSprite(const char *name) {
    int idx = HTableGet(&assetTable, name);
    return (0 <= idx && idx < assetCounts[ASSET_SPRITE]) ? assetSprites[idx]
                                                         : (Sprite){0};
}

Animation AssetsGetAnimation(const char *name) {
    int idx = HTableGet(&assetTable, name);
    return (0 <= idx && idx < assetCounts[ASSET_ANIMATION]) ? assetAnims[idx]
                                                            : (Animation){0};
}

Tile AssetsGetTile(int tileId) {
    return tileId < assetCounts[ASSET_TILE] ? assetTiles[tileId] : (Tile){0};
}

Map AssetGetMap(const char *name) {
    int idx = HTableGet(&assetTable, name);
    return (0 <= idx && idx < assetCounts[ASSET_MAP]) ? assetMaps[idx] : (Map){0};
}

void AssetsDestroy(void) {
    // clean up textures
    for (int i = 0; i < assetCounts[ASSET_TEXTURE]; ++i) {
        UnloadTexture(assetTextures[i]);
    }

    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning Asset arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}
