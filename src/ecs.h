#ifndef ECS_H
#define ECS_H

#include "raylib.h"

#define MAX_ENTITIES   1024
#define MAX_COMPONENTS 32

#define MAX_COMPONENT_TRANSFORM    1024
#define MAX_COMPONENT_RENDER       512
#define MAX_COMPONENT_SHAPE_RENDER 128
#define MAX_COMPONENT_ANIM_RENDER  128
#define MAX_COMPONENT_MAP_RENDER   4

#define NULL_ENTITY    -1
#define NULL_COMPONENT -1

typedef enum {
    COMPONENT_TRANSFORM = 0,
    COMPONENT_RENDER,
    COMPONENT_SHAPE_RENDER,
    COMPONENT_ANIMATION_RENDER,
    COMPONENT_MAP_RENDER,
    COMPONENT_COUNT
} CompType;

typedef struct Entity {
    bool enabled;
    int components[COMPONENT_COUNT];
} Entity;

typedef struct TransformComp {
    bool enabled;
    Vector2 position;
    Vector2 scale;
    float rotation;
} TransformComp;

int InitECS(void);

void DestroyECS(void);

int CreateEntity(void);

void RemoveEntity(int entityId);

void *CreateComponent(CompType compType, int entityId);

void RemoveComponent(CompType compType, int entityId);

void RegisterComponentCreate(CompType compType, int (*createCallback)(void **));

void RegisterComponentRemove(CompType compType, void (*removeCallback)(int));

#endif // !ECS_H
