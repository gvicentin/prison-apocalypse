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

    float blockSize = 32.0f;
    float defaultScale = 2.5f;

    SpriteRender player;
    player.sprite = AssetsGetSprite("policeman_idle_0");
    player.position = (Vector2){100, 100};
    player.scale = (Vector2){defaultScale, defaultScale};
    player.rotation = 0;
    player.tint = WHITE;

    MapRender map;
    map.map = AssetGetMap("prison");
    map.offset = Vector2Zero();
    map.tileSize = (Vector2){blockSize * defaultScale, blockSize * defaultScale};

    Camera2D camera = {0};
    camera.target =
        Vector2Add(player.position, (Vector2){blockSize * defaultScale * 0.5f,
                                              blockSize * defaultScale * 0.5f});
    camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    //----------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //------------------------------------------------------------------------------
        Vector2 input = Vector2Zero();
        float speed = 150;
        if (IsKeyDown(KEY_UP)) {
            input.y -= 1;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            input.x += 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            input.y += 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            input.x -= 1;
        }
        input = Vector2Normalize(input);
        Vector2 vel = Vector2Scale(input, speed * GetFrameTime());
        player.position = Vector2Add(player.position, vel);

        camera.target =
            Vector2Add(player.position, (Vector2){blockSize * defaultScale * 0.5f,
                                                  blockSize * defaultScale * 0.5f});
      // Camera rotation controls
        if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_S)) camera.rotation++;

        // Limit camera rotation to 80 degrees (-40 to 40)
        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40;

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 2.0f) camera.zoom = 2.0f;
        else if (camera.zoom < 0.5f) camera.zoom = 0.5f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R)) {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }
        //------------------------------------------------------------------------------

        // Draw
        //------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawMap(&map, camera.target);
        DrawSprite(&player);
        EndMode2D();
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
