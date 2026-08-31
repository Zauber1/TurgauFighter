#include "include/logic.h"

#include "raymath.h"
#include "include/GameLoop.h"

void UpdateGameState(GameState* state) {
    WalahiState* w = &state->walahi;

    Vector2 mouse = GetMousePosition();
    if (mouse.x != w->transform_2d.x.target_val || mouse.y != w->transform_2d.y.target_val) {
        Transform2DTo(&w->transform_2d, (Transform2DValue){
                .x = mouse.x,
                .y = mouse.y,
                .rotation = 0.0f,
                .scale = 1.0f,
        }, 0.2f);
    }

    Transform2DStep(&w->transform_2d);
}

void updateGameStateSubTick(GameState* state, float subtick_alpha) {
    // TODO increment walahi counter on sight:
    switch (state->current_page) {
        case SAY_WALAHI:
            break;
        case HOME:
            break;
            
        case ARENA:
            break;
    }
}