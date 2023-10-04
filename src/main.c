#include <math.h>
#include <stdlib.h>
#include "assets.h"
#include "ecs.h"
#include "utils.h"
#include "render.h"
#include "raylib.h"
#include "raymath.h"

static int CreatePlayer(Vector2 position, bool flip);
static int CreateGun(Vector2 position, bool flip);

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
    Vector2 centerPoint = {415, 270};
    int player = CreatePlayer(centerPoint, false);
    int gun = CreateGun(centerPoint, false);

    int map = CreateEntity();
    MapRenderComp *mapRenderComp = CreateComponent(COMPONENT_MAP_RENDER, map);
    mapRenderComp->tileWidth = 32;
    mapRenderComp->tileHeight = 32;
    mapRenderComp->renderLayersCount = 1;
    mapRenderComp->scale = (Vector2) {4, 4};
    mapRenderComp->map = AssetGetMap("prison");

    InitMapLayers(map);
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        TransformComp *gunTransf = GetComponent(COMPONENT_TRANSFORM, gun);
        RenderComp *gunRender = GetComponent(COMPONENT_RENDER, gun);
        RenderComp *playerRender = GetComponent(COMPONENT_RENDER, player);

        Vector2 mousePos = { GetMouseX(), GetMouseY() };
        Vector2 direction = Vector2Subtract(mousePos, centerPoint);
        float rotation = atan2f(direction.y, direction.x) * RAD2DEG;
        if (rotation > -90.0f && rotation < 90.0f) {
            gunRender->flipY = false;
            playerRender->flipX = false;
            gunRender->zOrder = 2;
        } else {
            gunRender->flipY = true;
            playerRender->flipX = true;
            gunRender->zOrder = 0;
        }
        gunTransf->rotation = rotation;
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateAnimationSystem(GetFrameTime());

        RenderMapLayerSystem(map, 0);
        RenderEntitiesSystem();

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

static int CreatePlayer(Vector2 position, bool flip) {
    int player = CreateEntity();

    TransformComp *playerTransf = CreateComponent(COMPONENT_TRANSFORM, player);
    playerTransf->position = position;
    playerTransf->scale = (Vector2) {4, 4};

    RenderComp *playerRender = CreateComponent(COMPONENT_RENDER, player);
    playerRender->sprite = AssetsGetSprite("policeman_idle_0");
    playerRender->pivot = (Vector2) {64.0f, 64.0f};
    playerRender->flipX = flip;

    AnimRenderComp *playerAnim = CreateComponent(COMPONENT_ANIMATION_RENDER, player);
    playerAnim->anim = AssetsGetAnimation("policeman_idle");

    return player;
}

static int CreateGun(Vector2 position, bool flip) {
    int gun = CreateEntity();

    TransformComp *gunTransf = CreateComponent(COMPONENT_TRANSFORM, gun);
    gunTransf->position = position;
    gunTransf->scale = (Vector2) {4, 4};

    RenderComp *gunRender = CreateComponent(COMPONENT_RENDER, gun);
    gunRender->sprite = AssetsGetSprite("rifle");
    gunRender->pivot = (Vector2) {15.0f, 15.0f};
    gunRender->flipX = flip;

    return gun;
}
