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
    WIN_SCREEN,
    GAMBLING,
} PAGE;

typedef struct WalahiState {
    Transform2D transform_2d;
} WalahiState;

#define TICKS_PER_SEC 60
#define MAX_HEALTH 100
#define STAGE_FLOOR_Y 640.0f

typedef struct PlayerState {
    Vector2 pos;
    float height;
    Vector2 velocity;
    Texture2D texture;
    bool fliped;
    Transform1D hit_x;
    Texture2D attack;
    int healt;
    int shader_id;
    bool owned_shaders[6];
} PlayerState;

typedef struct Buttons {
    int num;
    Rectangle* rect;
} Buttons;

typedef struct GamblingState {
    bool is_spinning;
    float spin_timer;
    float spin_duration;
    float reel_pos;
    float target_pos;
    int winning_shader;
    int winning_item_index;
    int last_click_index;
    bool has_won;
    int reel_items[64];
} GamblingState;

typedef struct GameState {
    PAGE current_page;
    uint32_t tick;
    WalahiState walahi;
    PlayerState player1;
    PlayerState player2;
    float run_time;
    Buttons buttons;
    int p1_wins;
    int p2_wins;
    int winner; // 1 = player 1, 2 = player 2
    float win_screen_timer;
    GamblingState gambling;
} GameState;

void UpdateGameState(GameState* state);
void updateGameStateSubTick(GameState* state, float subtick_alpha);
void reset_round(GameState* state);
void init_gambling_reel(GameState* state);
void start_gambling_spin(GameState* state);
void update_gambling_subtick(GameState* state, float dt);
#endif //TURGAUFIGHTER_LOGIC_H