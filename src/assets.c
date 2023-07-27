#include "assets.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ASSETS_PATH
#define ASSETS_PATH "./assets"
#endif
#define MAX_TEXTURES   8
#define MAX_SPRITES    32
#define MAX_ANIMATIONS 16
#define TABLE_CAPACITY 256
#define FNV_OFFSET     14695981039346656037UL
#define FNV_PRIME      1099511628211UL

typedef struct AssetTable_ {
    char *keysArray[TABLE_CAPACITY];
    size_t assetsArray[TABLE_CAPACITY];
} AssetTable;

static AssetTable assetTable = {0};

static Texture2D textures[MAX_TEXTURES] = {0};
static size_t textureCount = 0;

static Sprite sprites[MAX_SPRITES] = {0};
static size_t spriteCount = 0;

static Animation animations[MAX_ANIMATIONS] = {0};
static size_t animationsCount = 0;

static void addAssetToTable(const char *assetName, size_t assetId);
static size_t getAssetIdFromTable(const char *assetName);
static size_t hashKey(const char *key);

int AssetsInit(void) {
    // path where the assets will be loaded
    ChangeDirectory(ASSETS_PATH);

    return 0;
}

void AssetsDestroy(void) {
    for (size_t i = 0; i < textureCount; ++i) {
        UnloadTexture(textures[i]);
    }

    // deinitialize asset table
    for (size_t i = 0; i < TABLE_CAPACITY; ++i) {
        if (assetTable.keysArray[i] != NULL) {
            free(assetTable.keysArray[i]);
        }
    }
}

void AssetsLoadSpritesheet(const char *spritesheet) {
    char imageFilepath[256], metaFilepath[256];

    snprintf(imageFilepath, 256, "%s.png", spritesheet);
    snprintf(metaFilepath, 256, "%s.meta", spritesheet);

    // load from dist to GPU
    textures[textureCount] = LoadTexture(imageFilepath);

    char *metaContent = LoadFileText(metaFilepath);
    char *line_token = strtok(metaContent, "\n");
    while (line_token != NULL) {
        char sprite[32];
        int x, y, width, height, index;

        sscanf(line_token, "%31s %d %d %d %d %d", sprite, &x, &y, &width, &height,
               &index);
        printf("sprite: %s, x: %d, y: %d, w: %d, h: %d, i: %d\n", sprite, x, y, width,
               height, index);

        if (index == -1) {
            // creating sprite
            sprites[spriteCount] = (Sprite){&textures[textureCount], {x, y, width, height}};
            addAssetToTable(sprite, spriteCount++);
        } else {
            // TODO: create animation
        }

        line_token = strtok(NULL, "\n");
    }

    ++textureCount;
}

Sprite AssetsGetSprite(const char *sprite) {
    size_t spriteId = getAssetIdFromTable(sprite);
    return sprites[spriteId];
}

static void addAssetToTable(const char *assetName, size_t assetId) {
    uint64_t hash = hashKey(assetName);
    size_t index = hash % TABLE_CAPACITY;

    while (assetTable.keysArray[index] != NULL) {
        if (strcmp(assetName, assetTable.keysArray[index]) == 0) {
            // replace key
            assetTable.assetsArray[index] = assetId;
        }
        index = (index + 1) % TABLE_CAPACITY;
    }

    // duplicate string
    char *dupKey = (char *)malloc(strlen(assetName) + 1);
    strncpy(dupKey, assetName, strlen(assetName) + 1);

    // adding new entry
    assetTable.keysArray[index] = dupKey;
    assetTable.assetsArray[index] = assetId;
    printf("Adding assetId %lu on the index %lu\n", assetId, index);
}

static size_t getAssetIdFromTable(const char *assetName) {
    uint64_t hash = hashKey(assetName);
    size_t index = hash % TABLE_CAPACITY;

    while (assetTable.keysArray[index] != NULL) {
        if (strcmp(assetName, assetTable.keysArray[index]) == 0) {
            return assetTable.assetsArray[index];
        }
        index = (index + 1) % TABLE_CAPACITY;
    }

    return 0;
}

static uint64_t hashKey(const char *key) {
    // https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}
