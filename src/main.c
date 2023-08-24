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
    AssetAdd(ASSET_LOADER_SPRITESHEET, "prison");
    AssetAdd(ASSET_LOADER_MAP, "prison");

    int err = AssetLoadSync();
    if (err != 0) {
        TraceLog(LOG_ERROR, "Failed to load assets");
        return 1;
    }

    Sprite rifleSprite = AssetsGetSprite("rifle");
    Animation policemenAnim = AssetsGetAnimation("policeman_run");
    Sprite policemenSprite = policemenAnim.frames[0];
    Map *prisonMap = AssetGetMap("prison");

    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for (int y = 0; y < prisonMap->height; ++y) {
            for (int x = 0; x < prisonMap->width; ++x) {
                int tile = prisonMap->layers[0].tiles[y * prisonMap->width + x];
                Sprite tileSprite = prisonMap->tileset[tile];
                DrawTextureRec(tileSprite.tex, tileSprite.source, (Vector2) {32*x, 32*y}, WHITE);
            }
        }

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
