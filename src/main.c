#include "assets.h"
#include "ecs.h"
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
    AssetAdd(ASSET_LOADER_SPRITESHEET, "entities");
    AssetAdd(ASSET_LOADER_ANIMATION, "entities");
    AssetAdd(ASSET_LOADER_SPRITESHEET, "prison");
    AssetAdd(ASSET_LOADER_MAP, "prison");

    int err = AssetLoadSync();
    if (err != 0) {
        TraceLog(LOG_ERROR, "Failed to load assets");
        return 1;
    }

    SpriteRender player;
    player.sprite = AssetsGetSprite("policeman_idle_0");
    player.position = (Vector2) {100, 100};
    player.scale = (Vector2) {2, 2};
    player.rotation = 0;
    player.tint = WHITE;

    MapRender map;
    map.map = AssetGetMap("prison");
    map.offset = Vector2Zero();
    map.scale = (Vector2) {2, 2};
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //------------------------------------------------------------------------------
        Vector2 input = Vector2Zero();
        float speed = 100;
        if (IsKeyDown(KEY_UP)) {
           input.y += 1;
        }
        if (IsKeyDown(KEY_RIGHT)) {
           input.x -= 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
           input.y -= 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
           input.x += 1;
        }
        input = Vector2Normalize(input);
        Vector2 vel = Vector2Scale(input, speed * GetFrameTime());
        map.offset = Vector2Add(map.offset, vel);
        //------------------------------------------------------------------------------
        
        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawMap(&map);
        DrawSprite(&player);
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
