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

void DrawPlayer(PlayerState* player, float subtick_alpha) {
    float ratio = (float)player->texture.width / (float)player->texture.height;

    Rectangle src1 = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)player->texture.width * (player->fliped ? -1.0 : 1.0),
        .height = (float)player->texture.height,
    };
    Rectangle dest1 = {
        .x = player->pos.x,
        .y = player->pos.y,
        .width = player->height * ratio,
        .height = player->height,
    };
    Vector2 origin1 = { 0.0f, 0.0f };

    if (player->hit_x.is_active) {

        Rectangle src2 = {
            .x = 0.0,
            .y = 0.0,
            .width = player->attack.width * (player->fliped ? 1.0 : -1.0),
            .height = player->attack.height,
        };

        Rectangle dest2 = {
            .x = player->pos.x + Transform1DValueAt(&player->hit_x, subtick_alpha),
            .y = player->pos.y,
            .width = player->attack.width,
            .height = player->height,
        };

        DrawTexturePro(player->attack, src2, dest2, origin1, 0.0, WHITE);
    }

    DrawTexturePro(player->texture, src1, dest1, origin1, 0.0f, WHITE);
}

void draw_arena(GameState* state, float subtick_alpha) {
    DrawPlayer(&state->player1, subtick_alpha);
    DrawPlayer(&state->player2, subtick_alpha);
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

