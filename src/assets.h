#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>
#include <stdlib.h>

#define MAX_ANIM_FRAMES 4

typedef struct Sprite_ {
    Texture2D tex;
    Rectangle source;
} Sprite;

typedef struct Animation_ {
    Sprite frames[MAX_ANIM_FRAMES];
    size_t frameCount;
} Animation;

int AssetsInit(void);
void AssetsDestroy(void);

void AssetsLoadSpritesheet(const char *spritesFilepath);
void AssetsLoadAnimations(const char *animFilepath);

Sprite AssetsGetSprite(const char *sprite);
Animation AssetsGetAnimation(const char *animation);

#endif // !ASSETS_H
