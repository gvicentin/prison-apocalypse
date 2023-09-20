#ifndef ECS_H
#define ECS_H

#include <stdbool.h>
#include "raylib.h"
#include "assets.h"
#include "utils.h"

typedef enum {
    COMP_TRANSFORM = 0,
    COMP_SPRITERENDER,
    COMP_ANIMRENDER,
    COMP_MAPRENDER,
    COMP_CAMERA,
    COMP_PLAYER,
    COMP_GUN,
    COMP_COUNT
} CompType;

typedef struct Entity {
    bool enabled;
    int components[COMP_COUNT];
} Entity;

typedef struct TransformComp {
    bool enabled;

    Vector2 position;
    Vector2 scale;
    float rotation;
} TransformComp;

typedef struct SpriteRender {
    bool enabled;

    Sprite sprite;
    Vector2 pivot;
    Color tint;
    bool flipX, flipY;
} SpriteRender;

typedef struct AnimRender {
    bool enabled;

    Animation anim;
    float frameTime;
} AnimRender;

typedef struct MapRender {
    bool enabled;

    Map map;
    int tileWidth, tileHeight;
    int screenWidth, screenHeight;
    int renderLayersCount;
    RenderTexture2D renderLayers[MAX_MAP_LAYERS];
    Vector2 scale;
} MapRender;

typedef struct CameraComp {
    bool enabled;

    Camera2D camera;
    TransformComp *targetTransf;
    Vector2 offset;
} CameraComp;

typedef struct PlayerComp {
    bool enabled;

    float speed;
    Animation idleAnim;
    Animation runAnim;
} PlayerComp;

typedef struct GunComp {
    bool enabled;
    TransformComp *playerTransf;
    Vector2 offset;
} GunComp;

int EntityCompInit(void);
void EntityCompReset(void);
void EntityCompDestroy(void);

int EntityCreate(void);
void EntityRemove(int entityId);

void *ComponentCreate(int entityId, CompType type);
void ComponentRemove(int entityId, CompType type);

void SpriteRenderSetSprite(SpriteRender *spriteRender, Sprite sprite);

void SystemRenderEntities(AList *renderEntities);

void SystemAnimationUpdate(AList *animEntities, float dt);

void SystemMapInit(int mapEntity);
void SystemMapRenderLayer(int mapEntity, int layer);

void SystemCameraUpdate(int cameraEntity);

void SystemPlayerUpdate(int playerEntity, Vector2 input, Vector2 mousePos, float dt);

void SystemGunUpdate(int gunId, Vector2 mousePos, float dt);

#endif // !ECS_H
