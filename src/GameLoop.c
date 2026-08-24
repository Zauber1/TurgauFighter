//
// Created by timci on 24.08.26.
//

#include "include/GameLoop.h"
#include "raylib.h"
void GameLoop() {
    Texture2D texture_2d = LoadTexture("../assets/zero_walahis_left.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello",10,10,10,RAYWHITE);
        DrawTexture(texture_2d, 0, 0, WHITE);
        EndDrawing();
    }
}