#include "ecs.h"

#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>

#define ARENA_BUF_LEN Kilobyte(100)

static Arena arenaAlloc;

static Entity *entities;

static TransformComp *transformComponents;

static ComponentDef *componentDefs;

static int createTransformComponent(void **transfComp);
static void removeTransformComponent(int transfId);
static void *getTransformComponent(int transfId);

int InitECS(void) {
    void *backingBuffer = malloc(ARENA_BUF_LEN);
    ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
    if (backingBuffer == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
        return 1;
    }

    entities = ArenaAlloc(&arenaAlloc, sizeof(Entity) * MAX_ENTITIES);
    transformComponents = ArenaAlloc(&arenaAlloc, sizeof(TransformComp) * MAX_ENTITIES);
    componentDefs = ArenaAlloc(&arenaAlloc, sizeof(ComponentDef) * MAX_COMPONENTS);

    ComponentDef transformDef = {
        .createCallback = createTransformComponent,
        .removeCallback = removeTransformComponent,
        .getCallback = getTransformComponent
    };
    RegisterComponentDef(COMPONENT_TRANSFORM, transformDef);

    return 0;
}

void DestroyECS(void) {
    // free all arena at once
    TraceLog(LOG_DEBUG, "Cleaning ECS arena (%lu/%lu bytes used)",
             arenaAlloc.currOffset, ARENA_BUF_LEN);
    ArenaReset(&arenaAlloc);
    free(arenaAlloc.buff);
}

int CreateEntity(void) {
    int entityId;

    for (entityId = 0; entityId < MAX_ENTITIES; ++entityId) {
        if (!entities[entityId].enabled)
            break;
    }

    assert(entityId < MAX_ENTITIES);
    if (entityId >= MAX_ENTITIES) {
        return NULL_ENTITY;
    }

    entities[entityId].enabled = true;
    return entityId;
}

void RemoveEntity(int entityId) {
    if (entityId < 0 || entityId >= MAX_ENTITIES) {
        return;
    }

    Entity *removeEntity = &entities[entityId];
    removeEntity->enabled = false;
    for (int compType = 0; compType < MAX_COMPONENTS; ++compType) {
        if (removeEntity->components[compType] != NULL_COMPONENT) {
            RemoveComponent(entityId, compType);
            removeEntity->components[compType] = NULL_COMPONENT;
        }
    }
}

void *CreateComponent(CompType compType, int entityId) {
    int compId;
    void *component = NULL;

    int (*createComponent)(void **) = componentDefs[compType].createCallback;
    if (createComponent == NULL) {
        return NULL;
    }

    compId = createComponent(&component);
    entities[entityId].components[compType] = compId;

    return component;
}

void RemoveComponent(CompType compType, int entityId) {
    void (*removeComponent)(int) = componentDefs[compType].removeCallback;
    if (removeComponent != NULL) {
        int compId = entities[entityId].components[compType];
        removeComponent(compId);
    }
}

void *GetComponent(CompType compType, int entityId) {
    void *(*getComponent)(int) = componentDefs[compType].getCallback;
    if (getComponent != NULL) {
        int compId = entities[entityId].components[compType];
        return getComponent(compId);
    }
    return NULL;
}

void RegisterComponentDef(CompType compType, ComponentDef componentDef) {
    componentDefs[compType] = componentDef;
}

static int createTransformComponent(void **transfComp) {
    int compId;

    for (compId = 0; compId < MAX_COMPONENT_TRANSFORM; ++compId) {
        if (!transformComponents[compId].enabled)
            break;
    }

    assert(compId < MAX_COMPONENT_TRANSFORM);
    if (compId >= MAX_COMPONENT_TRANSFORM) {
        *transfComp = NULL;
        return NULL_COMPONENT;
    }

    transformComponents[compId] = (TransformComp){.enabled = true,
                                                  .position = Vector2Zero(),
                                                  .scale = Vector2One(),
                                                  .rotation = 0};

    *transfComp = &transformComponents[compId];
    return compId;
}

static void removeTransformComponent(int transfId) {
    transformComponents[transfId].enabled = false;
}

static void *getTransformComponent(int transfId) {
    return &transformComponents[transfId];
}

// #include "ecs.h"
//
// #include <assert.h>
// #include <math.h>
// #include <stdlib.h>
// #include <string.h>
//
// #include "assets.h"
// #include "raylib.h"
// #include "raymath.h"
// #include "utils.h"
//
// #define ARENA_BUF_LEN Kilobyte(500)
//
// #define MAX_ENTITIES     1024
// #define MAX_TRANSFORM    1024
// #define MAX_SPRITERENDER 1024
// #define MAX_ANIMRENDER   256
//
// #define NULL_ENTITY_COMP -1
//
// // Component specific functions
// static int createTransform(void **transformComp);
// static int createSpriteRender(void **spriteRender);
// static int createAnimRender(void **animRender);
// static int createMapRender(void **mapRender);
// static int createCamera(void **cameraComp);
// static int createPlayer(void **playerComp);
// static int createGun(void **gunComp);
//
// static void removeTransform(int transformCompId);
// static void removeSpriteRender(int spriteRenderId);
// static void removeAnimRender(int animRenderId);
// static void removeMapRender(int mapRenderId);
// static void removeCamera(int cameraCompId);
// static void removePlayer(int playerCompId);
// static void removeGun(int gunId);
//
//
// // entities
// static Entity *entities;
// static int entitiesCount;
//
// // components
// static TransformComp *compTransform;
// static SpriteRender *compSpriteRender;
// static AnimRender *compAnimRender;
// static MapRender *compMapRender;
// static CameraComp *compCamera;
// static PlayerComp *compPlayer;
// static GunComp *compGun;
//
// static int compCounts[COMP_COUNT];
//
// static int (*compCreateFP[])(void **) = {createTransform,  createSpriteRender,
//                                          createAnimRender, createMapRender,
//                                          createCamera,     createPlayer, createGun};
// static void (*compRemoveFP[])(int) = {removeTransform,  removeSpriteRender,
//                                       removeAnimRender, removeMapRender,
//                                       removeCamera,     removePlayer, removeGun};
//
// int EntityCompInit(void) {
//     void *backingBuffer = malloc(ARENA_BUF_LEN);
//     ArenaInit(&arenaAlloc, backingBuffer, ARENA_BUF_LEN);
//     if (backingBuffer == NULL) {
//         TraceLog(LOG_ERROR, "Failed to allocate memory for Arena");
//         return 1;
//     }
//
//     entities = ArenaAlloc(&arenaAlloc, sizeof(Entity) * MAX_ENTITIES);
//
//     compTransform = ArenaAlloc(&arenaAlloc, sizeof(TransformComp) * MAX_TRANSFORM);
//     compSpriteRender = ArenaAlloc(&arenaAlloc, sizeof(SpriteRender) *
//     MAX_SPRITERENDER); compAnimRender = ArenaAlloc(&arenaAlloc, sizeof(AnimRender) *
//     MAX_ANIMRENDER); compMapRender = ArenaAlloc(&arenaAlloc, sizeof(MapRender));
//     compCamera = ArenaAlloc(&arenaAlloc, sizeof(CameraComp));
//     compPlayer = ArenaAlloc(&arenaAlloc, sizeof(PlayerComp));
//     compGun = ArenaAlloc(&arenaAlloc, sizeof(GunComp));
//
//     EntityCompReset();
//
//     return 0;
// }
//
// void EntityCompReset(void) {
//     // reset all entities
//     entitiesCount = 0;
//     for (int entityId = 0; entityId < MAX_ENTITIES; ++entityId) {
//         Entity *entity = &entities[entityId];
//         entity->enabled = false;
//         for (int compId = 0; compId < COMP_COUNT; ++compId) {
//             entity->components[compId] = NULL_ENTITY_COMP;
//         }
//     }
//
//     // TODO: reset all components
//     memset(compCounts, 0, sizeof(compCounts));
// }
//
// void EntityCompDestroy(void) {
//     // free all arena at once
//     TraceLog(LOG_DEBUG, "Cleaning ECS arena (%lu/%lu bytes used)",
//              arenaAlloc.currOffset, ARENA_BUF_LEN);
//     ArenaReset(&arenaAlloc);
//     free(arenaAlloc.buff);
// }
//
// int EntityCreate(void) {
//     int entityId;
//
//     for (entityId = 0; entityId < MAX_ENTITIES; ++entityId) {
//         if (!entities[entityId].enabled)
//             break;
//     }
//
//     assert(entityId < MAX_ENTITIES);
//     if (entityId >= MAX_ENTITIES) {
//         return NULL_ENTITY_COMP;
//     }
//
//     entities[entityId].enabled = true;
//     return entityId;
// }
//
// void EntityRemove(int entityId) {
//     if (entityId < 0 || entityId >= MAX_ENTITIES) {
//         return;
//     }
//
//     Entity *removeEntity = &entities[entityId];
//     removeEntity->enabled = false;
//     for (int compType = 0; compType < COMP_COUNT; ++compType) {
//         ComponentRemove(entityId, compType);
//         removeEntity->components[compType] = NULL_ENTITY_COMP;
//     }
// }
//
// static int createTransform(void **transformComp) {
//     int compId;
//
//     for (compId = 0; compId < MAX_TRANSFORM; ++compId) {
//         if (!compTransform[compId].enabled)
//             break;
//     }
//
//     assert(compId < MAX_TRANSFORM);
//     if (compId >= MAX_TRANSFORM) {
//         *transformComp = NULL;
//         return NULL_ENTITY_COMP;
//     }
//
//     compTransform[compId] = (TransformComp){.enabled = true,
//                                             .position = Vector2Zero(),
//                                             .scale = Vector2One(),
//                                             .rotation = 0};
//
//     *transformComp = &compTransform[compId];
//     return compId;
// }
//
// static void removeTransform(int transformCompId) {
//     if (transformCompId >= 0 && transformCompId < MAX_TRANSFORM) {
//         compTransform[transformCompId].enabled = false;
//     }
// }
//
// static int createSpriteRender(void **spriteRender) {
//     int compId;
//
//     for (compId = 0; compId < MAX_SPRITERENDER; ++compId) {
//         if (!compSpriteRender[compId].enabled)
//             break;
//     }
//
//     assert(compId < MAX_SPRITERENDER);
//     if (compId >= MAX_SPRITERENDER) {
//         *spriteRender = NULL;
//         return NULL_ENTITY_COMP;
//     }
//
//     compSpriteRender[compId] = (SpriteRender){
//         .enabled = true, .sprite = {0}, .pivot = Vector2Zero(), .tint = WHITE, .flipX
//         = false, .flipY = false};
//
//     *spriteRender = &compSpriteRender[compId];
//     return compId;
// }
//
// static void removeSpriteRender(int spriteRenderId) {
//     if (spriteRenderId >= 0 && spriteRenderId < MAX_SPRITERENDER) {
//         compSpriteRender[spriteRenderId].enabled = false;
//     }
// }
//
// static int createAnimRender(void **animRender) {
//     int compId;
//
//     for (compId = 0; compId < MAX_ANIMRENDER; ++compId) {
//         if (!compAnimRender[compId].enabled)
//             break;
//     }
//
//     assert(compId < MAX_ANIMRENDER);
//     if (compId >= MAX_ANIMRENDER) {
//         *animRender = NULL;
//         return NULL_ENTITY_COMP;
//     }
//
//     compAnimRender[compId] =
//         (AnimRender){.enabled = true, .anim = {0}, .frameTime = 0.0f};
//
//     *animRender = &compAnimRender[compId];
//     return compId;
// }
//
// static void removeAnimRender(int animRenderId) {
//     if (animRenderId >= 0 && animRenderId < MAX_ANIMRENDER) {
//         compAnimRender[animRenderId].enabled = false;
//     }
// }
//
// static int createMapRender(void **mapRender) {
//     compMapRender->enabled = true;
//     compMapRender->map = (Map){0};
//     compMapRender->tileWidth = 0;
//     compMapRender->tileHeight = 0;
//     compMapRender->screenWidth = 0;
//     compMapRender->screenHeight = 0;
//     compMapRender->scale = Vector2One();
//     compMapRender->renderLayersCount = 0;
//     memset(compMapRender->renderLayers, 0, sizeof(compMapRender->renderLayers));
//     *mapRender = compMapRender;
//     return 0;
// }
//
// static void removeMapRender(int mapRenderId) {
//     if (mapRenderId >= 0) {
//         compMapRender->enabled = false;
//     }
// }
//
// static int createCamera(void **cameraComp) {
//     compCamera->enabled = true;
//     compCamera->camera =
//         (Camera2D){.target = Vector2Zero(), .offset = Vector2Zero(), .zoom = 1.0f};
//     compCamera->targetTransf = NULL;
//     compCamera->offset = Vector2Zero();
//     *cameraComp = compCamera;
//     return 0;
// }
//
// static void removeCamera(int cameraCompId) {
//     if (cameraCompId >= 0) {
//         compCamera->enabled = false;
//     }
// }
//
// static int createPlayer(void **playerComp) {
//     compPlayer->enabled = true;
//     compPlayer->speed = 0.0f;
//     compPlayer->idleAnim = (Animation){0};
//     compPlayer->runAnim = (Animation){0};
//     *playerComp = compPlayer;
//     return 0;
// }
//
// static void removePlayer(int playerCompId) {
//     if (playerCompId >= 0) {
//         compPlayer->enabled = false;
//     }
// }
//
// static int createGun(void **gunComp) {
//     compGun->enabled = true;
//     compGun->playerTransf = NULL;
//     *gunComp = compGun;
//     return 0;
// }
//
// static void removeGun(int gunId) {
//     if (gunId > 0) {
//         compGun->enabled = false;
//     }
// }
//
// void *ComponentCreate(int entityId, CompType type) {
//     int compId;
//     void *component = NULL;
//     compId = compCreateFP[type](&component);
//     entities[entityId].components[type] = compId;
//     return component;
// }
//
// void ComponentRemove(int entityId, CompType type) {
//     int compId = entities[entityId].components[type];
//     compRemoveFP[type](compId);
// }
//
// void SpriteRenderSetSprite(SpriteRender *spriteRender, Sprite sprite) {
//     spriteRender->sprite = sprite;
//     spriteRender->pivot = (Vector2) {sprite.source.width / 2.0f, sprite.source.height
//     / 2.0f };
// }
//
// void SystemMapInit(int mapEntity) {
//     int mapRenderId = entities[mapEntity].components[COMP_MAPRENDER];
//     if (mapRenderId == NULL_ENTITY_COMP) {
//         return;
//     }
//
//     MapRender *mapRender = compMapRender;
//     Map *map = &mapRender->map;
//     int layersCount = mapRender->renderLayersCount;
//
//     for (int layer = 0; layer < layersCount; ++layer) {
//         // create texture and enable for drawing
//         mapRender->renderLayers[layer] = LoadRenderTexture(
//             mapRender->tileWidth * map->width, mapRender->tileHeight * map->height);
//         BeginTextureMode(mapRender->renderLayers[layer]);
//
//         for (int y = 0; y < map->height; ++y) {
//             for (int x = 0; x < map->width; ++x) {
//                 int tileId = map->tiles[layer][y * map->width + x];
//                 Tile tile = AssetsGetTile(tileId);
//
//                 // draw tile to texture layer
//                 float invY = map->height - 1 - y;
//                 Rectangle src = tile.sprite.source;
//                 Rectangle dest = {x * mapRender->tileWidth,
//                                   invY * mapRender->tileHeight, src.width,
//                                   src.height};
//                 src.height = -src.height;
//                 DrawTexturePro(tile.sprite.tex, src, dest, Vector2Zero(), 0, WHITE);
//             }
//         }
//
//         EndTextureMode();
//     }
// }
//
// void SystemRenderEntities(AList *renderEntities) {
//     for (size_t entityId = 0; entityId < AListSize(renderEntities); ++entityId) {
//         int compTransId = entities[entityId].components[COMP_TRANSFORM];
//         int compSRId = entities[entityId].components[COMP_SPRITERENDER];
//         if (compTransId == NULL_ENTITY_COMP || compSRId == NULL_ENTITY_COMP) {
//             continue;
//         }
//
//         TransformComp *transfComp = &compTransform[compTransId];
//         SpriteRender *spriteRender = &compSpriteRender[compSRId];
//
//         Rectangle src = spriteRender->sprite.source;
//         src.width = spriteRender->flipX ? -src.width : src.width;
//         src.height = spriteRender->flipY ? -src.height : src.height;
//
//         Rectangle dest = {transfComp->position.x, transfComp->position.y,
//                           spriteRender->sprite.source.width * transfComp->scale.x,
//                           spriteRender->sprite.source.height * transfComp->scale.y};
//
//         Vector2 pivotCalc = Vector2Multiply(spriteRender->pivot, transfComp->scale);
//
//         DrawTexturePro(spriteRender->sprite.tex, src, dest, pivotCalc,
//                        transfComp->rotation, spriteRender->tint);
//     }
// }
//
// void SystemAnimationUpdate(AList *animEntities, float dt) {
//     for (size_t entityId = 0; entityId < AListSize(animEntities); ++entityId) {
//         int compSRId = entities[entityId].components[COMP_SPRITERENDER];
//         int compARId = entities[entityId].components[COMP_ANIMRENDER];
//         if (compARId == NULL_ENTITY_COMP || compSRId == NULL_ENTITY_COMP) {
//             continue;
//         }
//
//         SpriteRender *spriteRender = &compSpriteRender[compSRId];
//         AnimRender *animRender = &compAnimRender[compARId];
//
//         int frameCount = animRender->anim.frameCount;
//         float frameDur = animRender->anim.frameDuration;
//         int currentFrame = (int)(animRender->frameTime / frameDur) % frameCount;
//         animRender->frameTime += dt;
//         spriteRender->sprite = animRender->anim.frames[currentFrame];
//     }
// }
//
// void SystemMapRenderLayer(int mapEntity, int layer) {
//     int mapRenderId = entities[mapEntity].components[COMP_MAPRENDER];
//     if (mapRenderId == NULL_ENTITY_COMP) {
//         return;
//     }
//
//     MapRender *mapRender = compMapRender;
//     Texture2D layerTex = mapRender->renderLayers[layer].texture;
//
//     // draw map
//     Rectangle src = {0, 0, layerTex.width, layerTex.height};
//     Rectangle dest = {0, 0, layerTex.width * mapRender->scale.x,
//                       layerTex.height * mapRender->scale.y};
//     DrawTexturePro(layerTex, src, dest, Vector2Zero(), 0, WHITE);
// }
//
// void SystemCameraUpdate(int cameraEntity) {
//     int cameraCompId = entities[cameraEntity].components[COMP_CAMERA];
//     if (cameraCompId == NULL_ENTITY_COMP) {
//         return;
//     }
//
//     if (compCamera->targetTransf != NULL) {
//         compCamera->camera.target = compCamera->targetTransf->position;
//     }
//
//     compCamera->camera.offset = compCamera->offset;
// }
//
// void SystemPlayerUpdate(int playerEntity, Vector2 input, Vector2 mousePos, float dt)
// {
//     int transformCompId = entities[playerEntity].components[COMP_TRANSFORM];
//     int spriteCompId = entities[playerEntity].components[COMP_SPRITERENDER];
//     int animCompId = entities[playerEntity].components[COMP_ANIMRENDER];
//     int playerCompId = entities[playerEntity].components[COMP_PLAYER];
//     if (transformCompId == NULL_ENTITY_COMP || spriteCompId == NULL_ENTITY_COMP ||
//         animCompId == NULL_ENTITY_COMP || playerCompId == NULL_ENTITY_COMP) {
//         return;
//     }
//
//     TransformComp *transfComp = &compTransform[transformCompId];
//     SpriteRender *spriteComp = &compSpriteRender[spriteCompId];
//     AnimRender *animComp = &compAnimRender[animCompId];
//     PlayerComp *playerComp = compPlayer;
//
//     Vector2 vel = Vector2Scale(input, dt * playerComp->speed);
//     transfComp->position = Vector2Add(transfComp->position, vel);
//
//     if (Vector2Equals(vel, Vector2Zero())) {
//         animComp->anim = playerComp->idleAnim;
//     } else {
//         animComp->anim = playerComp->runAnim;
//     }
//
//     if (mousePos.x > 0) {
//         spriteComp->flipX = false;
//     } else if (mousePos.x < 0) {
//         spriteComp->flipX = true;
//     }
// }
//
// void SystemGunUpdate(int gunId, Vector2 mousePos, float dt) {
//     int gunTransformId = entities[gunId].components[COMP_TRANSFORM];
//     int gunSpriteRenderId = entities[gunId].components[COMP_SPRITERENDER];
//     if (gunTransformId == NULL_ENTITY_COMP || gunSpriteRenderId == NULL_ENTITY_COMP)
//     {
//         return;
//     }
//
//     TransformComp *gunTransform = &compTransform[gunTransformId];
//     SpriteRender *gunSpriteRender = &compSpriteRender[gunSpriteRenderId];
//
//     Vector2 offsetCalc = compGun->offset;
//     float rotation = atan2f(mousePos.y, mousePos.x) * RAD2DEG;
//
//     if (rotation > -90.0f && rotation < 90.0f) {
//         gunSpriteRender->flipY = false;
//     } else {
//         gunSpriteRender->flipY = true;
//     }
//
//     // gunTransform->position = Vector2Add(compGun->playerTransf->position,
//     offsetCalc); gunTransform->position = compGun->playerTransf->position;
//     gunTransform->rotation = rotation;
//
//     // DrawCircleV(, 2.0f, RED);
// }
