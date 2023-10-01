#ifndef RENDER_H
#define RENDER_H

#include "assets.h"
#include "ecs.h"
#include "raylib.h"

typedef enum {
    SHAPE_RENDER_RECT = 0,
    SHAPE_RENDER_FILLED_RECT,
    SHAPE_RENDER_CIRCLE,
    SHAPE_RENDER_CIRCLE_FILL,
    SHAPE_RENDER_COUNT
} ShapeRenderType;

typedef struct RenderComp {
    bool enabled;
    Sprite sprite;
    Vector2 pivot;
    Color tint;
    bool flipX, flipY;
} RenderComp;

typedef struct ShapeRenderComp {
    bool enabled;
    ShapeRenderType type;
    Rectangle rect;
    Vector2 center;
    float radius;
} ShapeRenderComp;

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

int InitRenderSystems(void);
void DestroyRenderSystems(void);

int CreateRenderComponent(void **renderComp);
int CreateShapeRenderComponent(void **shapeRenderComp);
int CreateAnimRenderComponent(void **animRenderComp);

void RemoveRenderComponent(int renderCompId);
void RemoveRenderShapeComponent(int shapeRenderCompId);
void RemoveAnimRenderComponent(int animRenderCompId);

void *GetRenderComponent(int renderCompId);
void *GetShapeRenderComponent(int shapeRenderCompId);
void *GetAnimationRenderComponent(int animRenderCompId);

void UpdateRenderSystem(void);
void UpdateAnimationSystem(float dt);

#endif // !RENDER_H
