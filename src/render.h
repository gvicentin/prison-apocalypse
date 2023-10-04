#ifndef RENDER_H
#define RENDER_H

#include "assets.h"
#include "ecs.h"
#include "raylib.h"

typedef struct RenderComp {
    bool enabled;
    Sprite sprite;
    Vector2 pivot;
    Color tint;
    bool flipX, flipY;
    int zOrder;
} RenderComp;

typedef struct AnimRenderComp {
    bool enabled;
    Animation anim;
    float frameTime;
} AnimRenderComp;

typedef struct MapRenderComp {
    bool enabled;
    int tileWidth, tileHeight;
    int screenWidth, screenHeight;
    Map map;
    int renderLayersCount;
    RenderTexture2D renderLayers[MAX_MAP_LAYERS];
    Vector2 scale;
} MapRenderComp;

typedef struct CameraComp {
    bool enabled;
    Camera2D camera;
} CameraComp;

int InitRenderSystems(void);
void DestroyRenderSystems(void);

int CreateRenderComponent(void **renderComp);
int CreateAnimRenderComponent(void **animRenderComp);
int CreateMapRenderComponent(void **mapRenderComp);

void RemoveRenderComponent(int renderCompId);
void RemoveAnimRenderComponent(int animRenderCompId);
void RemoveMapRenderComponent(int mapRenderCompId);

void *GetRenderComponent(int renderCompId);
void *GetAnimationRenderComponent(int animRenderCompId);
void *GetMapRenderComponent(int mapRenderCompId);

void UpdateAnimationSystem(float dt);
void RenderEntitiesSystem(void);

void InitMapLayers(int mapEntityId);
void RenderMapLayerSystem(int mapEntityId, int layer);

#endif // !RENDER_H
