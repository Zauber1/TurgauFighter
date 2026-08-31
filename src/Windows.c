#define WALAHI_COLOUR CLITERAL(Color){ 0, 0, 0, 255 }
#include <stdint.h>



#include "include/Windows.h"
#include "raylib.h"
#include <time.h>

#include "include/logic.h"

Texture2D walahi_texture;

void init_assets() {
    walahi_texture = LoadTexture("../assets/zero_walahis_left.png");
}

void draw_walahi(GameState* state, float subtick_alpha) {
    WalahiState* walahi = &state->walahi;

    Transform2DValue render_target = Transform2DValueAt(&walahi->transform_2d, subtick_alpha);

    int texture_width = walahi_texture.width;
    int texture_height = walahi_texture.height;
    Rectangle source = { 0.0f, 0.0f, (float)texture_width, (float)texture_height };
    Rectangle dest = { render_target.x, render_target.y, (float)texture_width * render_target.scale, (float)texture_height * render_target.scale };
    Vector2 origin = { (float)texture_width / 2.0f, (float)texture_height / 2.0f };

    DrawTexturePro(walahi_texture, source, dest, origin, render_target.rotation, WHITE);

}

void draw_home(GameState* state, float subtick_alpha) {

}

void draw_arena(GameState* state, float subtick_alpha) {
    PlayerState* player1 = &state->player1;
    PlayerState* player2 = &state->player2;

    bool flip_player1 = player1->velocity.x < 0.0;
    bool flip_player2 = player2->velocity.x < 0.0;

    Rectangle src1 = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)player1->texture.width * (player1->fliped ? -1.0 : 1.0),
        .height = (float)player1->texture.height
    };
    Rectangle dest1 = {
        .x = player1->pos.x,
        .y = player1->pos.y,
        .width = player1->size.x,
        .height = player1->size.y
    };
    Vector2 origin1 = { 0.0f, 0.0f };

    DrawTexturePro(player1->texture, src1, dest1, origin1, 0.0f, RED);

    Rectangle src2 = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)player2->texture.width * (player2->fliped ? -1.0 : 1.0),
        .height = (float)player2->texture.height
    };
    Rectangle dest2 = {
        .x = player2->pos.x,
        .y = player2->pos.y,
        .width = player2->size.x,
        .height = player2->size.y
    };
    Vector2 origin2 = { 0.0f, 0.0f };

    DrawTexturePro(player2->texture, src2, dest2, origin2, 0.0f, WHITE);
}

void DrawGame(GameState* state, float subtick_alpha) {
    BeginDrawing();
    ClearBackground(WALAHI_COLOUR);

    switch (state->current_page) {
        case SAY_WALAHI:
            draw_walahi(state, subtick_alpha);
            break;
        case HOME:
            draw_home(state, subtick_alpha);
            break;
        case ARENA:
            draw_arena(state, subtick_alpha);
            break;
    }

    EndDrawing();
}

