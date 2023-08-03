/* TODO: Assert number of instances and components */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "assets.h"
#include "raylib.h"
#include "raymath.h"

#define MAX_ENTITIES     512
#define MAX_COMPONENTS   32
#define MAX_TRANSFORMS   512
#define MAX_SPRITERENDER 512
#define EMPTY_COMPONENT  -1

#define GET_ECS_TABLE(compType, entityId)                                              \
    entitiesTable[(compType)*MAX_ENTITIES + (entityId)]
#define SET_ECS_TABLE(compType, entityId, compId)                                      \
    entitiesTable[(compType)*MAX_ENTITIES + (entityId)] = (compId)

//--------------------------------------------------------------------------------------
// Definitions
//--------------------------------------------------------------------------------------
typedef enum {
    COMP_TRANSFORM = 0,
    COMP_SPRITERENDER,
    COMP_ANIMATION,
    COMP_COUNT
} Components;

typedef struct TransformComp {
    int entityId;
    Vector2 position;
    Vector2 scale;
    float rotation;
} TransformComp;

typedef struct SpriteRender {
    int entityId;
    Sprite sprite;
    Vector2 size, pivot;
    Color tint;
    bool flipX, flipY;
} SpriteRender;

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
static int entitiesTable[MAX_ENTITIES * MAX_COMPONENTS];
static int entitiesCount;
static int player;

static TransformComp compTransform[MAX_ENTITIES];
static int compTransformCount;
static SpriteRender compSpriteRender[MAX_ENTITIES];
static int compSpriteRenderCount;

//--------------------------------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------------------------------
static int createPlayer(const char *spriteName, float x, float y);
static int createTrigger(float x, float y);
static void processSpriteRenderSystem(float dt);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    SetTraceLogLevel(LOG_DEBUG);

    // load assets
    AssetsInit();

    // ecs init
    for (int i = 0; i < MAX_ENTITIES * MAX_COMPONENTS; ++i) {
        entitiesTable[i] = EMPTY_COMPONENT;
    }
    entitiesCount = 0;
    compTransformCount = 0;
    compSpriteRenderCount = 0;
    memset(compTransform, 0, sizeof(compTransform));
    memset(compSpriteRender, 0, sizeof(compSpriteRender));

    // create entities
    for (int i = 0; i < 20; ++i) {
        // dummy entities
        createTrigger(i * 20, i * 20);
    }

    const char *spriteNames[] = {
        "policeman_idle_0",
        "policeman_die_0",
        "policeman_hit_0",
        "policeman_zombie_run_2",
        "prisoner_idle_0",
        "prisoner_zombie_run_0"
    };
    player = createPlayer("prisoner_zombie_run_0", 50, 50);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            createPlayer(spriteNames[GetRandomValue(0, 5)], i * 90, j * 90);
        }
    }

    for (int i = 1; i < compSpriteRenderCount; ++i) {
        compSpriteRender[i].flipX = GetRandomValue(0, 1);
        compSpriteRender[i].flipY = GetRandomValue(0, 1);
    }
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        float dt = GetFrameTime();
        Vector2 input = Vector2Zero();

        if (IsKeyDown(KEY_W)) {
            input.y -= 1;
        }
        if (IsKeyDown(KEY_D)) {
            input.x += 1;
        }
        if (IsKeyDown(KEY_S)) {
            input.y += 1;
        }
        if (IsKeyDown(KEY_A)) {
            input.x -= 1;
        }

        if (IsKeyPressed(KEY_Z)) {
            bool inverted = !compSpriteRender[GET_ECS_TABLE(COMP_SPRITERENDER, player)].flipX;
            compSpriteRender[GET_ECS_TABLE(COMP_SPRITERENDER, player)].flipX = inverted;
        }
        if (IsKeyPressed(KEY_X)) {
            bool inverted = !compSpriteRender[GET_ECS_TABLE(COMP_SPRITERENDER, player)].flipY;
            compSpriteRender[GET_ECS_TABLE(COMP_SPRITERENDER, player)].flipY = inverted;
        }

        Vector2 velocity = Vector2Scale(Vector2Normalize(input), dt * 100);
        compTransform[GET_ECS_TABLE(COMP_TRANSFORM, player)].position =
            Vector2Add(compTransform[player].position, velocity);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        processSpriteRenderSystem(dt);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    AssetsDestroy();
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

static int createPlayer(const char *spriteName, float x, float y) {
    int player = entitiesCount++;
    int transf = compTransformCount++;
    int sRender = compSpriteRenderCount++;
    Vector2 size = {64, 64};

    compTransform[transf] = (TransformComp){.entityId = player,
                                            .position = (Vector2){x, y},
                                            .scale = Vector2One(),
                                            .rotation = 0};
    compSpriteRender[sRender] =
        (SpriteRender){.entityId = player,
                       .sprite = AssetsGetSprite(spriteName),
                       .size = size,
                       .pivot = Vector2Scale(size, 0.5f),
                       .tint = WHITE,
                       .flipX = false,
                       .flipY = false};

    // update entities table
    SET_ECS_TABLE(COMP_TRANSFORM, player, transf);
    SET_ECS_TABLE(COMP_SPRITERENDER, player, sRender);

    return player;
}

static int createTrigger(float x, float y) {
    int trigger = entitiesCount++;
    int transf = compTransformCount++;

    compTransform[transf] = (TransformComp){.entityId = trigger,
                                            .position = (Vector2){x, y},
                                            .scale = Vector2One(),
                                            .rotation = 0};

    SET_ECS_TABLE(COMP_TRANSFORM, trigger, transf);

    return trigger;
}

static void processSpriteRenderSystem(float dt) {
    for (int i = 0; i < compSpriteRenderCount; ++i) {
        SpriteRender sRender = compSpriteRender[i];
        TransformComp transf =
            compTransform[GET_ECS_TABLE(COMP_TRANSFORM, sRender.entityId)];

        Vector2 mult = {sRender.flipX ? -1 : 1, sRender.flipY ? -1 : 1};
        Rectangle srcRect = sRender.sprite.source;
        srcRect.width *= mult.x;
        srcRect.height *= mult.y;
        Rectangle destRect = {transf.position.x, transf.position.y,
                          transf.scale.x * sRender.size.x,
                          transf.scale.y * sRender.size.y};
        DrawTexturePro(sRender.sprite.tex, srcRect, destRect, Vector2Zero(),
                       transf.rotation, sRender.tint);
    }
}
