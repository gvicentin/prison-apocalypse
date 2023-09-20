#include <stdlib.h>
#include "assets.h"
#include "ecs.h"
#include "utils.h"
#include "raylib.h"
#include "raymath.h"

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
    EntityCompInit();

    Arena arena = {0};
    AList renderEntities = {0};
    AList animEntities = {0};

    ArenaInit(&arena, malloc(Kilobyte(100)), Kilobyte(100));
    AListInit(&renderEntities, &arena);
    AListInit(&animEntities, &arena);

    int player = EntityCreate();

    TransformComp *playerTransf = ComponentCreate(player, COMP_TRANSFORM);
    playerTransf->position = (Vector2) {100, 100};
    playerTransf->scale = (Vector2) {2, 2};

    SpriteRender *playerSR = ComponentCreate(player, COMP_SPRITERENDER);
    SpriteRenderSetSprite(playerSR, AssetsGetSprite("policeman_idle_0"));
    AListAppend(&renderEntities, player);

    AnimRender *playerAR = ComponentCreate(player, COMP_ANIMRENDER);
    playerAR->anim = AssetsGetAnimation("policeman_idle");
    AListAppend(&animEntities, player);

    PlayerComp *playerComp = ComponentCreate(player, COMP_PLAYER);
    playerComp->speed = 150.0f;
    playerComp->idleAnim = AssetsGetAnimation("policeman_idle");
    playerComp->runAnim = AssetsGetAnimation("policeman_run");

    int gun = EntityCreate();
    
    TransformComp *gunTransf = ComponentCreate(gun, COMP_TRANSFORM);
    gunTransf->position = (Vector2) {100, 100};
    gunTransf->scale = (Vector2) {2, 2};

    SpriteRender *gunSR = ComponentCreate(gun, COMP_SPRITERENDER);
    SpriteRenderSetSprite(gunSR, AssetsGetSprite("rifle"));
    gunSR->pivot = (Vector2) {2, 2};

    AListAppend(&renderEntities, gun);

    GunComp *gunComp = ComponentCreate(gun, COMP_GUN);
    gunComp->playerTransf = playerTransf;
    gunComp->offset = (Vector2) {-5, 6};

    int map = EntityCreate();

    MapRender *mapRender = ComponentCreate(map, COMP_MAPRENDER);
    mapRender->map = AssetGetMap("prison");
    mapRender->tileWidth = 32;
    mapRender->tileHeight = 32;
    mapRender->scale = (Vector2) {2, 2};
    mapRender->renderLayersCount = 1;

    int camera = EntityCreate();
    
    CameraComp *cameraComp = ComponentCreate(camera, COMP_CAMERA);
    cameraComp->targetTransf = playerTransf;
    cameraComp->offset = (Vector2) {screenWidth/2.0f, screenHeight/2.0f};

    SystemMapInit(map);

    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //------------------------------------------------------------------------------
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

        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();
        Vector2 screenCenter = {screenWidth/2.0f, screenHeight/2.0f};
        Vector2 mouseCalc = Vector2Subtract(mousePos, screenCenter);

        SystemPlayerUpdate(player, Vector2Normalize(input), mouseCalc, dt);
        SystemAnimationUpdate(&animEntities, dt);
        SystemCameraUpdate(camera);
        SystemGunUpdate(gun, mouseCalc, dt);
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(cameraComp->camera);
        //SystemMapRenderLayer(map, 0);
        SystemRenderEntities(&renderEntities);
        EndMode2D();

        EndDrawing();
        //------------------------------------------------------------------------------
    }

    // De-Initialization
    //----------------------------------------------------------------------------------
    AssetsDestroy();
    EntityCompDestroy();
    free(arena.buff);
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------------------------------

    return 0;
}
