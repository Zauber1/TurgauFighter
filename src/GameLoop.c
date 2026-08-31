#include <stdint.h>

#include "raylib.h"

#include "include/GameLoop.h"
#include "include/Windows.h"
#include "include/logic.h"

void GameLoop() {
    init_assets();

    float accumulator = 0.0f;

    Texture2D attack = LoadTexture("../assets/damage.png");

    GameState state = {
        .current_page = ARENA,
        .tick = 0,
        .player1 = {
            .pos = {
                .x = 0,
                .y = 0,
            },
            .height = 100,
            .velocity = {
                .x = 0,
                .y = 0,
            },
            .texture = LoadTexture("../assets/ferris_knife.png"),
            .fliped = false,
            .hit_x = Transform1DCreate(0.0),
            .attack = attack,
            .healt = 100,
        },
        .player2 = {
            .pos = {
                .x = 0,
                .y = 0,
            },
            .height = 100,
            .velocity = {
                .x = 0,
                .y = 0,
            },
            .texture = LoadTexture("../assets/ferris.png"),
            .fliped = false,
            .hit_x = Transform1DCreate(0.0),
            .attack = attack,
            .healt = 100,
        },
        .walahi = {
            .transform_2d = Transform2DCreate((Transform2DValue){
                .x = 0.0f,
                .y = 0.0f,
                .rotation = 0.0f,
                .scale = 1.0f,
            }),
        },
        .run_time = 0.0f,
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.25f) { dt = 0.25f; }

        accumulator += dt;
        state.run_time += dt;

        while (accumulator >= dELTAtIME) {
            UpdateGameState(&state);
            state.tick++;
            accumulator -= dELTAtIME;
        }

        float subtick_alpha = accumulator / dELTAtIME;

        updateGameStateSubTick(&state, dt);
        DrawGame(&state, subtick_alpha);
    }
}