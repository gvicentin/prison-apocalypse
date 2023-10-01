#include <stdlib.h>
#include "assets.h"
#include "ecs.h"
#include "utils.h"
#include "render.h"
#include "raylib.h"
#include "raymath.h"

static int CreatePlayer(Vector2 position);
static int CreateGun(Vector2 position);

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

    AssetsInit();
    
    // init assets
    AssetAdd(ASSET_LOADER_SPRITESHEET, "entities");
    AssetAdd(ASSET_LOADER_ANIMATION, "entities");
    AssetAdd(ASSET_LOADER_SPRITESHEET, "prison");
    AssetAdd(ASSET_LOADER_MAP, "prison");

    int err = AssetLoadSync();
    if (err != 0) {
        TraceLog(LOG_ERROR, "Failed to load assets");
        return 1;
    }

    // init ecs
    InitECS();
    InitRenderSystems();

    // player 1
    CreatePlayer((Vector2) {200, 200});
    int gun = CreateGun((Vector2) {200, 200});

    // player 2
    int player = CreatePlayer((Vector2) {500, 200});
    CreateGun((Vector2) {500, 200});

    RemoveEntity(gun);
    RemoveEntity(player);

    int newGun = CreateGun((Vector2) {100, 50});
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GRAY);

        UpdateAnimationSystem(GetFrameTime());
        UpdateRenderSystem();

        EndDrawing();
        //------------------------------------------------------------------------------
    }

    // De-Initialization
    //----------------------------------------------------------------------------------
    DestroyECS();
    DestroyRenderSystems();
    AssetsDestroy();
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------------------------------

    return 0;
}

static int CreatePlayer(Vector2 position) {
    int player = CreateEntity();

    TransformComp *playerTransf = CreateComponent(COMPONENT_TRANSFORM, player);
    playerTransf->position = position;
    playerTransf->scale = (Vector2) {4, 4};

    RenderComp *playerRender = CreateComponent(COMPONENT_RENDER, player);
    playerRender->sprite = AssetsGetSprite("policeman_idle_0");

    AnimRenderComp *playerAnim = CreateComponent(COMPONENT_ANIMATION_RENDER, player);
    playerAnim->anim = AssetsGetAnimation("policeman_idle");

    return player;
}

static int CreateGun(Vector2 position) {
    int gun = CreateEntity();

    TransformComp *gunTransf = CreateComponent(COMPONENT_TRANSFORM, gun);
    gunTransf->position = position;
    gunTransf->scale = (Vector2) {4, 4};

    RenderComp *gunRender = CreateComponent(COMPONENT_RENDER, gun);
    gunRender->sprite = AssetsGetSprite("rifle");
    gunRender->pivot = (Vector2) {0.2f, 0.6f};

    return gun;
}
