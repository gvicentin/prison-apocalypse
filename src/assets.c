#include "assets.h"
#include "raylib.h"

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

typedef struct AssetTable {
    char *keysArray[TABLE_CAPACITY];
    int assetsArray[TABLE_CAPACITY];
} AssetTable;

static AssetTable assetTable = {0};
static Texture2D textures[MAX_TEXTURES] = {0};
static Sprite sprites[MAX_SPRITES] = {0};
static Animation animations[MAX_ANIMATIONS] = {0};
static int textureCount = 0;
static int spriteCount = 0;
static int animationsCount = 0;

static void addAssetToTable(const char *assetName, int assetId);
static int getAssetIdFromTable(const char *assetName);
static uint64_t hashKey(const char *key);

int AssetsInit(void) {
    // path where the assets will be loaded
    ChangeDirectory(ASSETS_PATH);

    AssetsLoadSpritesheet("entities");
    AssetsLoadAnimations("entities");

    TraceLog(LOG_DEBUG, "Textures loaded: %u/%u", textureCount, MAX_TEXTURES);
    TraceLog(LOG_DEBUG, "Sprites loaded: %u/%u", spriteCount, MAX_SPRITES);
    TraceLog(LOG_DEBUG, "Animations loaded: %u/%u", animationsCount, MAX_ANIMATIONS);

    return 0;
}

void AssetsDestroy(void) {
    for (int i = 0; i < textureCount; ++i) {
        UnloadTexture(textures[i]);
    }

    // deinitialize asset table
    for (int i = 0; i < TABLE_CAPACITY; ++i) {
        if (assetTable.keysArray[i] != NULL) {
            free(assetTable.keysArray[i]);
        }
    }
}

void AssetsLoadSpritesheet(const char *spritesheet) {
    char imageFilepath[256], metaFilepath[256];

    snprintf(imageFilepath, 256, "%s.png", spritesheet);
    snprintf(metaFilepath, 256, "%s.sprite", spritesheet);

    // load from dist to GPU
    textures[textureCount] = LoadTexture(imageFilepath);

    char *metaContent = LoadFileText(metaFilepath);
    char *line_token = strtok(metaContent, "\n");
    while (line_token != NULL) {
        char sprite[32];
        int x, y, width, height;

        // creating sprite
        sscanf(line_token, "%31s %d %d %d %d", sprite, &x, &y, &width, &height);
        sprites[spriteCount] = (Sprite){textures[textureCount], {x, y, width, height}};
        addAssetToTable(sprite, spriteCount++);

        line_token = strtok(NULL, "\n");
    }
    ++textureCount;

    // cleanup
    free(metaContent);
}

void AssetsLoadAnimations(const char *anim) {
    char animFilepath[256];

    snprintf(animFilepath, 256, "%s.anim", anim);

    char *animContent = LoadFileText(animFilepath);
    char *line_token = strtok(animContent, "\n");
    while (line_token != NULL) {
        char animName[32], spriteName[36];
        int frameCount;

        sscanf(line_token, "%31s %d", animName, &frameCount);
        animations[animationsCount].frameCount = frameCount;
        for (int i = 0; i < frameCount; ++i) {
            sprintf(spriteName, "%s_%d", animName, i);
            animations[animationsCount].frames[i] = AssetsGetSprite(spriteName);
        }

        addAssetToTable(animName, animationsCount++);
        line_token = strtok(NULL, "\n");
    }
    
    // cleanup
    free(animContent);
}

Sprite AssetsGetSprite(const char *sprite) {
    int spriteId = getAssetIdFromTable(sprite);
    return sprites[spriteId];
}

Animation AssetsGetAnimation(const char *anim) {
    int animId = getAssetIdFromTable(anim);
    return animations[animId];
}

static void addAssetToTable(const char *assetName, int assetId) {
    uint64_t hash = hashKey(assetName);
    int index = hash % TABLE_CAPACITY;

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
}

static int getAssetIdFromTable(const char *assetName) {
    uint64_t hash = hashKey(assetName);
    int index = hash % TABLE_CAPACITY;

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
