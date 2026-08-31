#define WALAHI_COLOUR CLITERAL(Color){ 0, 0, 0, 255 }
#include <stdint.h>

#include <stdio.h>
#include "include/ButtonControls.h"


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
    Rectangle button1 = (Rectangle){10,10,100,30};
    DrawRectangle(button1.x,button1.y,button1.width,button1.height,RAYWHITE);
    ClearBackground(BLACK);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        switch (checkMouseClick(1,GetMousePosition(),button1)) {
            case 0:
                state->current_page=SAY_WALAHI;
                break;
            default:
                printf("NEOEEEEEEE");
                break;
        }
    }

}

void draw_arena(GameState* state, float subtick_alpha) {

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


