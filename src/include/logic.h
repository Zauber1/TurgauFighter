//
// Created by levin on 24.08.2026.
//

#ifndef TURGAUFIGHTER_LOGIC_H
#define TURGAUFIGHTER_LOGIC_H
#include <stdint.h>

#include "raylib.h"
#include "transform.h"

typedef enum PAGE {
    SAY_WALAHI,
    HOME,
    ARENA,
} PAGE;

typedef struct WalahiState {
    Transform2D transform_2d;
} WalahiState;

#define TICKS_PER_SEC 60

typedef struct PlayerState {
    Vector2 pos;
    Vector2 size;
    Vector2 velocity;
    Texture2D texture;
    bool fliped;
} PlayerState;

typedef struct GameState {
    PAGE current_page;
    uint32_t tick;
    WalahiState walahi;
    PlayerState player1;
    PlayerState player2;
    float run_time;
} GameState;

void UpdateGameState(GameState* state);
void updateGameStateSubTick(GameState* state, float subtick_alpha);
#endif //TURGAUFIGHTER_LOGIC_H