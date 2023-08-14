#include <assert.h>
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
#define MAX_ANIMATIONS   64
#define EMPTY_COMPONENT  -1

#define MAX_ANIM_FRAMES 4

#define DEFAULT_SIZE                                                                   \
    (Vector2) { 64, 64 }
#define DEFAULT_SCALE                                                                  \
    (Vector2) { 2, 2 }

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
} ComponentType;

typedef struct TransformComp {
    int entityId;
    Vector2 position;
    Vector2 scale;
    float rotation;
} TransformComp;

typedef struct SpriteRender {
    int entityId;
    Sprite sprite;
    Vector2 size, pivot;    // TODO: remove?
    Color tint;
    bool flipX, flipY;
} SpriteRender;

typedef struct Animation {
    int entityId;
    AnimationSprite animSprite;
    int currentFrame;
    float frameDuration, frameTime;
} Animation;

//--------------------------------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------------------------------
static int createEntity(void);
static int createComponent(ComponentType compType, int entity);
static void initTransform(int transf, int entity);
static void initSpriteRender(int sRender, int entity);
static void initAnimation(int anim, int entity);

static int createCharacter(const char *spriteName, float x, float y);
static int createPlayer(float x, float y);
static int createTrigger(float x, float y);

static void processSpriteRenderSystem(void);
static void processAnimation(float dt);

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
static int entitiesTable[MAX_ENTITIES * MAX_COMPONENTS];
static int entitiesCount;
static int player;

static TransformComp compTransform[MAX_TRANSFORMS];
static SpriteRender compSpriteRender[MAX_SPRITERENDER];
static Animation compAnimation[MAX_ANIMATIONS];

static int compCounts[MAX_COMPONENTS];
static int maxComponents[] = {MAX_TRANSFORMS, MAX_SPRITERENDER, MAX_ANIMATIONS};
static void (*componentInits[])(int, int) = {initTransform, initSpriteRender,
                                             initAnimation};

//--------------------------------------------------------------------------------------
// Program main entry point
//--------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //----------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Prison Apocalypse");

    SetTargetFPS(60);
    SetTraceLogLevel(LOG_DEBUG);

    // load assets
    AssetsInit();

    // ecs init
    for (int i = 0; i < MAX_ENTITIES * MAX_COMPONENTS; ++i) {
        entitiesTable[i] = EMPTY_COMPONENT;
    }
    entitiesCount = 0;

    // create entities
    for (int i = 0; i < 20; ++i) {
        // dummy entities
        createTrigger(i * 20, i * 20);
    }

    const char *spriteNames[] = {"medical_bag", "ammo_bag",
                                 "machine_gun",  "policeman_zombie_run_2",
                                 "prisoner_idle_0",  "prisoner_zombie_run_0"};
    player = createPlayer(50, 50);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            createCharacter(spriteNames[GetRandomValue(0, 5)], i * 90, j * 90);
        }
    }

    for (int i = 1; i < compCounts[COMP_SPRITERENDER]; ++i) {
        compSpriteRender[i].flipX = GetRandomValue(0, 1);
        compSpriteRender[i].flipY = GetRandomValue(0, 1);
    }
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //------------------------------------------------------------------------------
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

        TransformComp *playerTransf =
            &compTransform[GET_ECS_TABLE(COMP_TRANSFORM, player)];
        Animation *playerAnim = &compAnimation[GET_ECS_TABLE(COMP_ANIMATION, player)];
        SpriteRender *playerRender =
            &compSpriteRender[GET_ECS_TABLE(COMP_SPRITERENDER, player)];

        Vector2 velocity = Vector2Scale(Vector2Normalize(input), dt * 100);
        playerTransf->position = Vector2Add(compTransform[player].position, velocity);

        if (Vector2Equals(velocity, Vector2Zero())) {
            playerAnim->animSprite = AssetsGetAnimation("policeman_idle");
            playerAnim->frameDuration = 0.2f;
        } else {
            playerAnim->animSprite = AssetsGetAnimation("policeman_run");
            playerAnim->frameDuration = 0.09f;
        }

        if (velocity.x < 0) {
            playerRender->flipX = true;
        } else if (velocity.x > 0) {
            playerRender->flipX = false;
        }

        // update systems
        processAnimation(dt);
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        processSpriteRenderSystem();
        EndDrawing();
        //------------------------------------------------------------------------------
    }

    // De-Initialization
    //----------------------------------------------------------------------------------
    AssetsDestroy();
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------------------------------

    return 0;
}

static int createEntity(void) {
    assert(entitiesCount + 1 < MAX_ENTITIES);
    return entitiesCount++;
}

static int createComponent(ComponentType compType, int entity) {
    assert(compCounts[compType] + 1 < maxComponents[compType]);
    int comp = compCounts[compType]++;

    SET_ECS_TABLE(compType, entity, comp);
    componentInits[compType](comp, entity);

    return comp;
}

static void initTransform(int transf, int entity) {
    compTransform[transf].entityId = entity;
    compTransform[transf].position = Vector2Zero();
    compTransform[transf].scale = DEFAULT_SCALE;
    compTransform[transf].rotation = 0;
}

static void initSpriteRender(int sRender, int entity) {
    compSpriteRender[sRender].entityId = entity;
    compSpriteRender[sRender].sprite = (Sprite){0};
    compSpriteRender[sRender].size = Vector2Zero();
    compSpriteRender[sRender].pivot = Vector2Zero();
    compSpriteRender[sRender].tint = WHITE;
    compSpriteRender[sRender].flipX = false;
    compSpriteRender[sRender].flipY = false;
}

static void initAnimation(int anim, int entity) {
    compAnimation[anim].entityId = entity;
    compAnimation[anim].animSprite = (AnimationSprite){0};
    compAnimation[anim].currentFrame = 0;
    compAnimation[anim].frameTime = 0;
    compAnimation[anim].frameDuration = 0.2f;
}

static int createCharacter(const char *spriteName, float x, float y) {
    int character = createEntity();
    int transf = createComponent(COMP_TRANSFORM, character);
    int sRender = createComponent(COMP_SPRITERENDER, character);

    compTransform[transf].position = (Vector2){x, y};
    compSpriteRender[sRender].sprite = AssetsGetSprite(spriteName);

    return character;
}

static int createPlayer(float x, float y) {
    int player = createEntity();
    int transf = createComponent(COMP_TRANSFORM, player);
    int sRender = createComponent(COMP_SPRITERENDER, player);
    int anim = createComponent(COMP_ANIMATION, player);

    compTransform[transf].position = (Vector2){x, y};
    compSpriteRender[sRender].sprite = AssetsGetSprite("policeman_idle_0");
    compAnimation[anim].animSprite = AssetsGetAnimation("policeman_run");
    compAnimation[anim].frameDuration = 0.1;

    return player;
}

static int createTrigger(float x, float y) {
    int trigger = createEntity();
    int transf = createComponent(COMP_TRANSFORM, player);

    compTransform[transf].position = (Vector2){x, y};

    return trigger;
}

static void processSpriteRenderSystem(void) {
    for (int i = 0; i < compCounts[COMP_SPRITERENDER]; ++i) {
        SpriteRender sRender = compSpriteRender[i];
        TransformComp transf =
            compTransform[GET_ECS_TABLE(COMP_TRANSFORM, sRender.entityId)];

        Vector2 mult = {sRender.flipX ? -1 : 1, sRender.flipY ? -1 : 1};
        Rectangle srcRect = sRender.sprite.source;
        srcRect.width *= mult.x;
        srcRect.height *= mult.y;
        Rectangle destRect = {transf.position.x, transf.position.y,
                              transf.scale.x * sRender.sprite.source.width,
                              transf.scale.y * sRender.sprite.source.height};
        DrawTexturePro(sRender.sprite.tex, srcRect, destRect, Vector2Zero(),
                       transf.rotation, sRender.tint);
    }
}

static void processAnimation(float dt) {
    for (int i = 0; i < compCounts[COMP_ANIMATION]; ++i) {
        Animation *anim = &compAnimation[i];
        SpriteRender *sRender =
            &compSpriteRender[GET_ECS_TABLE(COMP_ANIMATION, anim->entityId)];

        anim->frameTime += dt;
        anim->currentFrame = ((int)(anim->frameTime / anim->frameDuration)) %
                             anim->animSprite.frameCount;
        sRender->sprite = anim->animSprite.frames[anim->currentFrame];
    }
}
