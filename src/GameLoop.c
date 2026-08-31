#include <stdint.h>

#include "raylib.h"

#include "include/GameLoop.h"
#include "include/Windows.h"
#include "include/logic.h"

void GameLoop() {
    init_assets();

    float accumulator = 0.0f;

    GameState state = {
        .current_page = SAY_WALAHI,
        .tick = 0,
        .player1 = {
            .pos = {
                .x = 0,
                .y = 0,
            },
        },
        .player2 = {
            .pos = {
                .x = 0,
                .y = 0,
            },
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
        updateGameStateSubTick(&state, subtick_alpha);
        DrawGame(&state, subtick_alpha);
    }
}