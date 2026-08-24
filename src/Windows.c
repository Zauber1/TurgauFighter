#define WALAHI_COLOUR CLITERAL(Color){ 0, 0, 0, 255 }

typedef enum PAGE {
    SAY_WALAHI,
    HOME,
    ARENA,
} PAGE;

#include "include/Windows.h"
#include "raylib.h"

Texture2D walahi_texture;

void init_assets() {
    walahi_texture = LoadTexture("../assets/zero_walahis_left.png");
}

void draw_walahi(PAGE* current_page) {
    DrawTexture(walahi_texture, 0, 0, WHITE);
}

void draw_home(PAGE* current_page) {

}

void draw_arena(PAGE* current_page) {

}

void MainMenu() {
    PAGE current_page = SAY_WALAHI;

    BeginDrawing();
    ClearBackground(WALAHI_COLOUR);

    switch (current_page) {
        case SAY_WALAHI:
            draw_walahi(&current_page);
            break;
        case HOME:
            draw_home(&current_page);
            break;
        case ARENA:
            draw_arena(&current_page);
            break;
    }

    EndDrawing();
}

