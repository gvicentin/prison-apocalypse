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
    EntityComponentInit();

    Arena arena = {0};
    AList renderEntities = {0};
    AList animEntities = {0};

    ArenaInit(&arena, malloc(Kilobyte(100)), Kilobyte(100));
    AListInit(&renderEntities, &arena);
    AListInit(&animEntities, &arena);

    int player = EntityCreate();
    SpriteRender *playerSR = ComponentCreate(player, COMP_SPRITERENDER);
    playerSR->scale = (Vector2) {2, 2};
    AnimRender *playerAR = ComponentCreate(player, COMP_ANIMRENDER);
    playerAR->anim = AssetsGetAnimation("policeman_idle");
    AListAppend(&renderEntities, player);
    AListAppend(&animEntities, player);

    int gun = EntityCreate();
    SpriteRender *gunSR = ComponentCreate(gun, COMP_SPRITERENDER);
    gunSR->sprite = AssetsGetSprite("rifle");
    gunSR->scale = (Vector2) {2, 2};
    gunSR->position = (Vector2) {100, 100};
    AListAppend(&renderEntities, gun);

    int map = EntityCreate();
    MapRender *mapRender = ComponentCreate(map, COMP_MAPRENDER);
    mapRender->map = AssetGetMap("prison");
    mapRender->screenSize = (Vector2) {screenWidth, screenHeight};
    mapRender->tileSize = (Vector2) {32, 32};
    mapRender->scale = (Vector2) {2, 2};
    mapRender->renderLayersCount = 1;

    SystemMapInit(map);

    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        //------------------------------------------------------------------------------

        SystemAnimationUpdate(&animEntities, GetFrameTime());
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        EndMode2D();
        SystemMapRenderLayer(map, 0);
        SystemRenderEntities(&renderEntities);
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
