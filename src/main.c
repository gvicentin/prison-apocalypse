#include <stdio.h>

#include "assets.h"
#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    AssetsInit();
    AssetsLoadSpritesheet("policemen_character");

    Sprite policemanSprite = AssetsGetSprite("policemen_die");
    printf("%.2f %.2f\n", policemanSprite.source.x, policemanSprite.source.y);

    Sprite policemanSprite2 = AssetsGetSprite("policemen_iddle");
    printf("%.2f %.2f\n", policemanSprite2.source.x, policemanSprite2.source.y);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        DrawText("Congrats! You created your first window!", 190, 200, 20, WHITE);

        DrawTextureRec(*(policemanSprite2.tex), policemanSprite2.source,
                       (Vector2){100, 50}, WHITE);

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
