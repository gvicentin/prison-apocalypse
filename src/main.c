#include <stdbool.h>
#include <stdio.h>

#include "assets.h"
#include "raylib.h"
#include "raymath.h"

#define MAX_ENTITIES 32

typedef struct TransformComp {
    Vector2 position;
    Vector2 scale;
    float rotation;
} TransformComp;

typedef struct RenderComp {
    Sprite sprite;
    Vector2 size, pivot;
    Color tint;
    bool flipX, flipY;
} RenderComp;

typedef struct AnimComp {
    Animation anim;
    float frameTime;
} AnimComp;

static int entitiesCount = 0;
static TransformComp transforms[MAX_ENTITIES];
static RenderComp renderes[MAX_ENTITIES];

static int player;

static int createPlayer(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    SetTraceLogLevel(LOG_DEBUG);

    AssetsInit();

    // creating player
    player = createPlayer();
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
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

        Vector2 velocity = Vector2Scale(Vector2Normalize(input), dt * 100);
        transforms[player].position = Vector2Add(transforms[player].position, velocity);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for (int i = 0; i < entitiesCount; ++i) {
            TransformComp t = transforms[i];
            RenderComp r = renderes[i];

            Rectangle dest = {t.position.x, t.position.y, t.scale.x * r.size.x,
                              t.scale.y * r.size.y};
            DrawTexturePro(r.sprite.tex, r.sprite.source, dest, r.pivot, t.rotation,
                           r.tint);

            Rectangle debugRect = dest;
            debugRect.x -= r.pivot.x;
            debugRect.y -= r.pivot.y;
            DrawRectangleLinesEx(debugRect, 1, RED);
            DrawCircleV(t.position, 2, RED);
        }

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

static int createPlayer(void) {
    int player = entitiesCount++;
    Vector2 size = {64, 64};

    transforms[player] = (TransformComp){
        .position = Vector2Zero(), .scale = Vector2One(), .rotation = 0};
    renderes[player] = (RenderComp){.sprite = AssetsGetSprite("policeman_idle_0"),
                                    .size = size,
                                    .pivot = Vector2Scale(size, 0.5f),
                                    .tint = WHITE,
                                    .flipX = false,
                                    .flipY = false};
    return player;
}
