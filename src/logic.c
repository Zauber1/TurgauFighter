#include "include/logic.h"

#include "raymath.h"
#include "include/GameLoop.h"

void UpdateGameState(GameState* state) {

    switch (state->current_page) {
        case SAY_WALAHI:
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
        case HOME:
            break;
        case ARENA:
            Transform1DStep(&state->player1.hit_x);
            Transform1DStep(&state->player2.hit_x);
            break;
    }

}

static void update_player_physics(PlayerState* player, float move_input, bool jump_input, bool attack_input, float dt) {
    const float ACCELERATION  = 3500.0f; // px/s^2
    const float MAX_SPEED     = 600.0f;  // px/s
    const float FRICTION      = 12.0f;   // Drag coefficient
    const float GRAVITY       = 2000.0f; // px/s^2 (tuning value in screen pixels)
    const float JUMP_FORCE    = -800.0f; // Initial upward velocity burst

    float screen_height = (float)GetScreenHeight();
    float floor_y = screen_height - player->height;

    if (move_input != 0.0f) {
        bool move_negative = move_input < 0.0;
        bool velocity_negative = player->velocity.x < 0.0;

        if (move_negative == velocity_negative) {
            player->velocity.x += move_input * ACCELERATION * dt;
            player->velocity.x = Clamp(player->velocity.x, -MAX_SPEED, MAX_SPEED);
        } else {
            player->velocity.x = move_input * ACCELERATION * dt;
            player->velocity.x = Clamp(player->velocity.x, -MAX_SPEED, MAX_SPEED);
        }

    } else {
        player->velocity.x = Lerp(player->velocity.x, 0.0f, FRICTION * dt);
        if (fabsf(player->velocity.x) < 1.0f) {
            player->velocity.x = 0.0f;
        }
    }

    bool is_grounded = (player->pos.y >= floor_y);

    if (jump_input && is_grounded) {
        player->velocity.y = JUMP_FORCE;
        is_grounded = false;
    }

    if (!is_grounded) {
        player->velocity.y += GRAVITY * dt;
    }

    player->pos.x += player->velocity.x * dt;
    player->pos.y += player->velocity.y * dt;

    if (player->pos.y >= floor_y) {
        player->pos.y = floor_y;
        player->velocity.y = 0.0f;
    }

    if (player->velocity.x != 0.0) {
        if (player->velocity.x < 0) {
            player->fliped = true;
        } else {
            player->fliped = false;
        }
    }

    if (attack_input) {
        float ratio = (float)player->texture.width / (float)player->texture.height;
        float width = player->height * ratio;

        float start_offset  = player->fliped ? (-player->attack.width) : (width);
        float target_offset = player->fliped ? (-player->attack.width - 20.0f) : (width + 20.0f);

        player->hit_x = Transform1DCreate(start_offset);
        Transform1DTo(&player->hit_x, target_offset, 0.1f);
    }
}

void update_arena_subtick(GameState* state, float dt) {
    PlayerState* player1 = &state->player1;
    PlayerState* player2 = &state->player2;

    // (-1.0 to 1.0)
    float p1_move = (float)IsKeyDown(KEY_D) - (float)IsKeyDown(KEY_A);
    bool p1_jump = IsKeyDown(KEY_W);
    bool p1_attack = IsKeyPressed(KEY_E);

    float p2_move = (float)IsKeyDown(KEY_L) - (float)IsKeyDown(KEY_J);
    bool p2_jump = IsKeyDown(KEY_I);
    bool p2_attack = IsKeyPressed(KEY_O);

    update_player_physics(player1, p1_move, p1_jump, p1_attack, dt);
    update_player_physics(player2, p2_move, p2_jump, p2_attack, dt);
}

void updateGameStateSubTick(GameState* state, float dt) {
    // increment walahi counter on sight: 1
    switch (state->current_page) {
        case SAY_WALAHI:
            break;
        case HOME:
            break;
        case ARENA:
            update_arena_subtick(state, dt);
            break;
    }
}