#include <stdint.h>

#include "raylib.h"

#include "include/init.h"
#include "include/GameLoop.h"
#include "include/Windows.h"
#include "include/logic.h"
#include "include/MonkeySpriteSheetDefinitionWriter.h"
#include "include/shaders.h"

void onPageChange(PAGE old, PAGE new) {

}

void GameLoop() {

    runWriter(817, 763, 900, 0, 62);

    init_assets();
    InitShaders();

    float accumulator = 0.0f;
    Rectangle rect[4] = {
        { 260, 240, 280, 52 },
        { 260, 310, 280, 52 },
        { 260, 380, 280, 52 },
        { 0, 0, 0, 0 }
    };
    Texture2D attack = LoadGameTexture("damage.png");

    GameState state = {
        .current_page = HOME,
        .tick = 0,
        .player1 = {
            .pos = {
                .x = 120,
                .y = STAGE_FLOOR_Y - 100.0f,
            },
            .height = 100,
            .velocity = {
                .x = 0,
                .y = 0,
            },
            .texture = LoadGameTexture("ferris_knife.png"),
            .fliped = false,
            .hit_x = Transform1DCreate(0.0f),
            .attack = attack,
            .healt = MAX_HEALTH,
            .shader_id = SHADER_NONE,
            .owned_shaders = { true, false, false, false, false, false },
        },
        .player2 = {
            .pos = {
                .x = 550,
                .y = STAGE_FLOOR_Y - 100.0f,
            },
            .height = 100,
            .velocity = {
                .x = 0,
                .y = 0,
            },
            .texture = LoadGameTexture("ferris.png"),
            .fliped = true,
            .hit_x = Transform1DCreate(0.0f),
            .attack = attack,
            .healt = MAX_HEALTH,
            .shader_id = SHADER_NONE,
            .owned_shaders = { true, false, false, false, false, false },
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
        .buttons = {
            .num = 3,
            .rect = rect,
        },
        .p1_wins = 0,
        .p2_wins = 0,
        .winner = 0,
        .win_screen_timer = 0.0f,
    };

    init_gambling_reel(&state);

    PAGE current_page = state.current_page;

    while (!WindowShouldClose()) {
        if (current_page != state.current_page) {
            onPageChange(current_page, state.current_page);
            current_page = state.current_page;
        }

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

    UnloadAllShaders();
}