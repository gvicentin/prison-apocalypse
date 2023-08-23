#include "assets.h"
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
    AssetLoadSync();

    Sprite rifleSprite = AssetsGetSprite("rifle");
    Animation policemenAnim = AssetsGetAnimation("policeman_run");
    Sprite policemenSprite = policemenAnim.frames[0];

    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        DrawTextureRec(rifleSprite.tex, rifleSprite.source, (Vector2) {50, 50}, WHITE);
        DrawTextureRec(policemenSprite.tex, policemenSprite.source, (Vector2) {50, 50}, WHITE);

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
