#include "include/logic.h"
#include "include/shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raymath.h"
#include "include/GameLoop.h"

void reset_round(GameState* state) {
    float ground_y = STAGE_FLOOR_Y - state->player1.height;

    state->player1.pos = (Vector2){ 120.0f, ground_y };
    state->player1.velocity = (Vector2){ 0.0f, 0.0f };
    state->player1.fliped = false;
    state->player1.hit_x = Transform1DCreate(0.0f);
    state->player1.healt = MAX_HEALTH;

    state->player2.pos = (Vector2){ 550.0f, ground_y };
    state->player2.velocity = (Vector2){ 0.0f, 0.0f };
    state->player2.fliped = true;
    state->player2.hit_x = Transform1DCreate(0.0f);
    state->player2.healt = MAX_HEALTH;

    state->winner = 0;
    state->win_screen_timer = 0.0f;
}

void UpdateGameState(GameState* state) {
    switch (state->current_page) {
        case SAY_WALAHI: {
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
            break;
        }
        case HOME:
        case GAMBLING:
            break;
        case ARENA:
        case WIN_SCREEN:
            Transform1DStep(&state->player1.hit_x);
            Transform1DStep(&state->player2.hit_x);
            break;
    }
}

static void update_player_physics(PlayerState* player, PlayerState* enemy, float move_input, bool jump_input, bool attack_input, float dt) {
    const float ACCELERATION  = 3500.0f; // px/s^2
    const float MAX_SPEED     = 550.0f;  // px/s
    const float FRICTION      = 12.0f;   // Drag coefficient
    const float GRAVITY       = 2000.0f; // px/s^2 (tuning value in screen pixels)
    const float JUMP_FORCE    = -800.0f; // Initial upward velocity burst

    float screen_width = (float)GetScreenWidth();
    float ground_y = STAGE_FLOOR_Y - player->height;

    float ratio = (player->texture.height > 0) ? ((float)player->texture.width / (float)player->texture.height) : 1.0f;
    float player_width = player->height * ratio;

    if (move_input != 0.0f) {
        player->velocity.x += move_input * ACCELERATION * dt;
        player->velocity.x = Clamp(player->velocity.x, -MAX_SPEED, MAX_SPEED);
    } else {
        player->velocity.x = Lerp(player->velocity.x, 0.0f, FRICTION * dt);
        if (fabsf(player->velocity.x) < 1.0f) {
            player->velocity.x = 0.0f;
        }
    }

    bool is_grounded = (player->pos.y >= ground_y);

    if (jump_input && is_grounded) {
        player->velocity.y = JUMP_FORCE;
        is_grounded = false;
    }

    if (!is_grounded) {
        player->velocity.y += GRAVITY * dt;
    }

    player->pos.x += player->velocity.x * dt;
    player->pos.y += player->velocity.y * dt;

    if (player->pos.y >= ground_y) {
        player->pos.y = ground_y;
        player->velocity.y = 0.0f;
    }

    // Keep player within the screen boundaries
    if (player->pos.x < 0.0f) {
        player->pos.x = 0.0f;
        player->velocity.x = 0.0f;
    }
    if (player->pos.x > screen_width - player_width) {
        player->pos.x = screen_width - player_width;
        player->velocity.x = 0.0f;
    }

    if (player->velocity.x < -10.0f) {
        player->fliped = true;
    } else if (player->velocity.x > 10.0f) {
        player->fliped = false;
    }

    // Attack action and damage
    if (attack_input && !player->hit_x.is_active) {
        float start_offset  = player->fliped ? (-(float)player->attack.width) : (player_width);
        float target_offset = player->fliped ? (-(float)player->attack.width - 25.0f) : (player_width + 25.0f);

        player->hit_x = Transform1DCreate(start_offset);
        Transform1DTo(&player->hit_x, target_offset, 0.15f);

        // Attack hitbox calculation based on facing direction
        float attack_w = (float)player->attack.width + 25.0f;
        float attack_x = player->fliped ? (player->pos.x - attack_w) : (player->pos.x + player_width);
        float attack_y = player->pos.y;
        float attack_h = player->height;

        float enemy_ratio = (enemy->texture.height > 0) ? ((float)enemy->texture.width / (float)enemy->texture.height) : 1.0f;
        float enemy_width = enemy->height * enemy_ratio;

        Rectangle attack_rec = { attack_x, attack_y, attack_w, attack_h };
        Rectangle enemy_rec = { enemy->pos.x, enemy->pos.y, enemy_width, enemy->height };

        if (CheckCollisionRecs(attack_rec, enemy_rec)) {
            enemy->healt -= 15;
            if (enemy->healt < 0) {
                enemy->healt = 0;
            }
            // Add knockback to make the hit feel punchy
            float kb_dir = player->fliped ? -1.0f : 1.0f;
            enemy->velocity.x += kb_dir * 320.0f;
            enemy->velocity.y -= 160.0f;
        }
    }
}

void update_arena_subtick(GameState* state, float dt) {
    PlayerState* player1 = &state->player1;
    PlayerState* player2 = &state->player2;

    // Check if either player is defeated
    if (player1->healt <= 0 || player2->healt <= 0) {
        if (player1->healt <= 0 && player2->healt <= 0) {
            state->winner = 1;
            state->p1_wins++;
        } else if (player2->healt <= 0) {
            state->winner = 1;
            state->p1_wins++;
        } else {
            state->winner = 2;
            state->p2_wins++;
        }
        state->win_screen_timer = 3.0f;
        state->current_page = WIN_SCREEN;
        return;
    }

    // Player 1 controls: A/D Move, W Jump, E Attack
    float p1_move = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A);
    bool p1_jump = IsKeyDown(KEY_W);
    bool p1_attack = IsKeyPressed(KEY_E);

    // Player 2 controls: J/L Move, I Jump, O Attack
    float p2_move = (float)IsKeyDown(KEY_L) - (float)IsKeyDown(KEY_J);
    bool p2_jump = IsKeyDown(KEY_I);
    bool p2_attack = IsKeyPressed(KEY_O);

    update_player_physics(player1, player2, p1_move, p1_jump, p1_attack, dt);
    update_player_physics(player2, player1, p2_move, p2_jump, p2_attack, dt);

    if (IsKeyPressed(KEY_ESCAPE)) {
        state->current_page = HOME;
    }
}

void updateGameStateSubTick(GameState* state, float dt) {
    switch (state->current_page) {
        case SAY_WALAHI:
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                state->current_page = HOME;
            }
            break;
        case HOME:
            break;
        case ARENA:
            update_arena_subtick(state, dt);
            break;
        case WIN_SCREEN:
            state->win_screen_timer -= dt;
            if (state->win_screen_timer <= 0.0f || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                reset_round(state);
                state->current_page = ARENA;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                state->current_page = HOME;
            }
            break;
        case GAMBLING:
            update_gambling_subtick(state, dt);
            break;
    }
}

void init_gambling_reel(GameState* state) {
    state->gambling.is_spinning = false;
    state->gambling.spin_timer = 0.0f;
    state->gambling.spin_duration = 5.0f;
    state->gambling.reel_pos = 0.0f;
    state->gambling.target_pos = 0.0f;
    state->gambling.winning_shader = 0;
    state->gambling.winning_item_index = 0;
    state->gambling.last_click_index = -1;
    state->gambling.has_won = false;

    // Fill reel items with shaders (weighted distribution for excitement)
    // 1: Glint (common/restricted), 2: Mandelbrot, 3: Holo, 4: Gold (rare!), 5: Fire
    for (int i = 0; i < 64; i++) {
        int r = rand() % 100;
        if (r < 25) {
            state->gambling.reel_items[i] = SHADER_GLINT;
        } else if (r < 50) {
            state->gambling.reel_items[i] = SHADER_HOLO;
        } else if (r < 75) {
            state->gambling.reel_items[i] = SHADER_MANDELBROT;
        } else if (r < 90) {
            state->gambling.reel_items[i] = SHADER_FIRE;
        } else {
            state->gambling.reel_items[i] = SHADER_GOLD; // Ultra rare gold!
        }
    }
}

void start_gambling_spin(GameState* state) {
    if (state->gambling.is_spinning) return;

    // Pick a winning index between 35 and 48
    int win_idx = 35 + (rand() % 14);
    state->gambling.winning_item_index = win_idx;
    state->gambling.winning_shader = state->gambling.reel_items[win_idx];

    // Card width is 130 + 10 margin = 140 step
    // Subtle jitter within the center of the card (-30 to +30 px)
    float jitter = (float)((rand() % 61) - 30);
    state->gambling.target_pos = (float)win_idx * 140.0f + jitter;

    state->gambling.is_spinning = true;
    state->gambling.spin_timer = 0.0f;
    state->gambling.spin_duration = 5.0f;
    state->gambling.reel_pos = 0.0f;
    state->gambling.last_click_index = -1;
    state->gambling.has_won = false;
}

void update_gambling_subtick(GameState* state, float dt) {
    if (state->gambling.is_spinning) {
        state->gambling.spin_timer += dt;
        float t = state->gambling.spin_timer / state->gambling.spin_duration;
        if (t >= 1.0f) {
            t = 1.0f;
            state->gambling.is_spinning = false;
            state->gambling.has_won = true;
            state->gambling.reel_pos = state->gambling.target_pos;
        } else {
            // Quartic ease-out: rapid spin with gradual, suspenseful slowdown
            float ease = 1.0f - powf(1.0f - t, 4.0f);
            state->gambling.reel_pos = state->gambling.target_pos * ease;
        }

        // Trigger click sound when an item passes the center marker (needle)
        int current_idx = (int)((state->gambling.reel_pos + 70.0f) / 140.0f);
        if (current_idx != state->gambling.last_click_index && current_idx >= 0) {
            state->gambling.last_click_index = current_idx;
            PlayReelClickSound();
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        state->current_page = HOME;
    }
}