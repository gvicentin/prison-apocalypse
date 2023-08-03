#ifndef ASSETS_H
#define ASSETS_H

#include <raylib.h>

#define MAX_ANIM_FRAMES 4

typedef struct Sprite {
    Texture2D tex;
    Rectangle source;
} Sprite;

typedef struct AnimationSprite {
    Sprite frames[MAX_ANIM_FRAMES];
    int frameCount;
} AnimationSprite;

int AssetsInit(void);
void AssetsDestroy(void);

void AssetsLoadSpritesheet(const char *spritesheet);
void AssetsLoadAnimations(const char *anim);

Sprite AssetsGetSprite(const char *sprite);
AnimationSprite AssetsGetAnimation(const char *animation);

#endif // !ASSETS_H
