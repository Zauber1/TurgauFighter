#define WALAHI_COLOUR CLITERAL(Color){ 0, 0, 0, 255 }
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "include/ButtonControls.h"
#include "include/Windows.h"
#include "include/logic.h"
#include "include/init.h"

void draw_arena(GameState* state, float subtick_alpha);

Texture2D walahi_texture;

void init_assets() {
    walahi_texture = LoadGameTexture("zero_walahis_left.png");
}

void draw_walahi(GameState* state, float subtick_alpha) {
    ClearBackground(BLACK);

    WalahiState* walahi = &state->walahi;
    Transform2DValue render_target = Transform2DValueAt(&walahi->transform_2d, subtick_alpha);

    int texture_width = walahi_texture.width;
    int texture_height = walahi_texture.height;
    if (texture_width > 0 && texture_height > 0) {
        Rectangle source = { 0.0f, 0.0f, (float)texture_width, (float)texture_height };
        Rectangle dest = { render_target.x, render_target.y, (float)texture_width * render_target.scale, (float)texture_height * render_target.scale };
        Vector2 origin = { (float)texture_width / 2.0f, (float)texture_height / 2.0f };
        DrawTexturePro(walahi_texture, source, dest, origin, render_target.rotation, WHITE);
    }

    DrawText("WALAHI MODE", 20, 20, 24, RAYWHITE);
    DrawText("Move mouse to swing Walahi | Press [ESC] to return to Home", 20, 55, 16, GRAY);
}

void draw_home(GameState* state, float subtick_alpha) {
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    ClearBackground((Color){ 16, 18, 25, 255 });

    // Decorative grid lines in background
    for (int y = 0; y < screen_h; y += 40) {
        DrawLine(0, y, screen_w, y, (Color){ 24, 27, 38, 80 });
    }
    for (int x = 0; x < screen_w; x += 40) {
        DrawLine(x, 0, x, screen_h, (Color){ 24, 27, 38, 80 });
    }

    // Title with stylish shadow
    const char* title = "TURGAU FIGHTER";
    int title_size = 54;
    int title_w = MeasureText(title, title_size);
    int title_x = (screen_w - title_w) / 2;
    int title_y = 75;

    DrawText(title, title_x + 3, title_y + 3, title_size, (Color){ 5, 5, 10, 240 });
    DrawText(title, title_x, title_y, title_size, (Color){ 245, 190, 45, 255 });

    const char* subtitle = "Rust Crab vs C++ Crab -- Battle for Memory Safety";
    int sub_w = MeasureText(subtitle, 18);
    DrawText(subtitle, (screen_w - sub_w) / 2, title_y + 65, 18, (Color){ 175, 185, 205, 255 });

    // Character preview artwork
    if (state->player1.texture.id != 0) {
        float r1 = (float)state->player1.texture.width / (float)state->player1.texture.height;
        DrawTexturePro(state->player1.texture,
            (Rectangle){ 0.0f, 0.0f, (float)state->player1.texture.width, (float)state->player1.texture.height },
            (Rectangle){ 50.0f, 210.0f, 130.0f * r1, 130.0f },
            (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        int p1_lbl_w = MeasureText("Rust Crab", 16);
        DrawText("Rust Crab", 50 + (int)(130.0f * r1 - p1_lbl_w) / 2, 350, 16, (Color){ 100, 175, 255, 255 });
    }

    if (state->player2.texture.id != 0) {
        float r2 = (float)state->player2.texture.width / (float)state->player2.texture.height;
        float p2_drawn_w = 130.0f * r2;
        DrawTexturePro(state->player2.texture,
            (Rectangle){ 0.0f, 0.0f, -(float)state->player2.texture.width, (float)state->player2.texture.height },
            (Rectangle){ (float)screen_w - 50.0f - p2_drawn_w, 210.0f, p2_drawn_w, 130.0f },
            (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        int p2_lbl_w = MeasureText("C++ Crab", 16);
        DrawText("C++ Crab", screen_w - 50 - (int)p2_drawn_w + (int)(p2_drawn_w - p2_lbl_w) / 2, 350, 16, (Color){ 255, 210, 65, 255 });
    }

    // Home buttons
    Rectangle btn_arena = state->buttons.rect[0];
    Rectangle btn_walahi = state->buttons.rect[1];

    Vector2 mouse = GetMousePosition();
    bool hover1 = CheckCollisionPointRec(mouse, btn_arena);
    bool hover2 = CheckCollisionPointRec(mouse, btn_walahi);

    // Button 1: PLAY ARENA
    Color b1_bg = hover1 ? (Color){ 45, 95, 200, 255 } : (Color){ 28, 55, 125, 245 };
    Color b1_border = hover1 ? (Color){ 130, 195, 255, 255 } : (Color){ 65, 125, 230, 255 };
    DrawRectangleRounded(btn_arena, 0.25f, 4, b1_bg);
    DrawRectangleRoundedLinesEx(btn_arena, 0.25f, 4, 2, b1_border);

    const char* b1_text = "PLAY ARENA";
    int b1_w = MeasureText(b1_text, 22);
    DrawText(b1_text, btn_arena.x + (btn_arena.width - b1_w) / 2, btn_arena.y + 17, 22, WHITE);

    // Button 2: SAY WALAHI
    Color b2_bg = hover2 ? (Color){ 175, 130, 20, 255 } : (Color){ 110, 80, 15, 245 };
    Color b2_border = hover2 ? (Color){ 255, 225, 80, 255 } : (Color){ 210, 165, 35, 255 };
    DrawRectangleRounded(btn_walahi, 0.25f, 4, b2_bg);
    DrawRectangleRoundedLinesEx(btn_walahi, 0.25f, 4, 2, b2_border);

    const char* b2_text = "SAY WALAHI";
    int b2_w = MeasureText(b2_text, 22);
    DrawText(b2_text, btn_walahi.x + (btn_walahi.width - b2_w) / 2, btn_walahi.y + 17, 22, WHITE);

    // Controls explanation panel
    Rectangle ctrl_panel = { (screen_w - 560) / 2, 455, 560, 255 };
    DrawRectangleRounded(ctrl_panel, 0.12f, 4, (Color){ 24, 27, 38, 240 });
    DrawRectangleRoundedLinesEx(ctrl_panel, 0.12f, 4, 1, (Color){ 55, 62, 85, 255 });

    const char* ctrl_hdr = "CONTROLS & GUIDE";
    int hdr_w = MeasureText(ctrl_hdr, 18);
    DrawText(ctrl_hdr, ctrl_panel.x + (ctrl_panel.width - hdr_w) / 2, ctrl_panel.y + 16, 18, (Color){ 230, 235, 250, 255 });

    DrawText("Rust Crab (Blue):", ctrl_panel.x + 35, ctrl_panel.y + 52, 16, (Color){ 100, 175, 255, 255 });
    DrawText("[A] [D] Walk    [W] Jump    [E] Attack Knife", ctrl_panel.x + 55, ctrl_panel.y + 76, 15, WHITE);

    DrawText("C++ Crab (Yellow):", ctrl_panel.x + 35, ctrl_panel.y + 112, 16, (Color){ 255, 210, 65, 255 });
    DrawText("[J] [L] Walk    [I] Jump    [O] Attack C++ Strike", ctrl_panel.x + 55, ctrl_panel.y + 136, 15, WHITE);

    DrawLine(ctrl_panel.x + 30, ctrl_panel.y + 175, ctrl_panel.x + ctrl_panel.width - 30, ctrl_panel.y + 175, (Color){ 50, 56, 75, 255 });
    DrawText("Hit opponent to deplete HP! First to 0 loses the round.", ctrl_panel.x + 35, ctrl_panel.y + 190, 14, (Color){ 175, 185, 205, 255 });
    DrawText("Round score counters at top center track wins (CS2 style)!", ctrl_panel.x + 35, ctrl_panel.y + 215, 14, (Color){ 175, 185, 205, 255 });

    // Handle mouse click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int clicked = checkMouseClick(2, mouse, btn_arena, btn_walahi);
        switch (clicked) {
            case 0:
                reset_round(state);
                state->current_page = ARENA;
                break;
            case 1:
                state->current_page = SAY_WALAHI;
                break;
            default:
                break;
        }
    }
}

void DrawPlayer(PlayerState* player, float subtick_alpha) {
    float ratio = (player->texture.height > 0) ? ((float)player->texture.width / (float)player->texture.height) : 1.0f;
    float p_width = player->height * ratio;

    Rectangle src1 = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)player->texture.width * (player->fliped ? -1.0f : 1.0f),
        .height = (float)player->texture.height,
    };
    Rectangle dest1 = {
        .x = player->pos.x,
        .y = player->pos.y,
        .width = p_width,
        .height = player->height,
    };
    Vector2 origin1 = { 0.0f, 0.0f };

    // Draw attack visual if active
    if (player->hit_x.is_active && player->attack.id != 0) {
        Rectangle src2 = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)player->attack.width * (player->fliped ? 1.0f : -1.0f),
            .height = (float)player->attack.height,
        };

        Rectangle dest2 = {
            .x = player->pos.x + Transform1DValueAt(&player->hit_x, subtick_alpha),
            .y = player->pos.y,
            .width = (float)player->attack.width,
            .height = player->height,
        };

        DrawTexturePro(player->attack, src2, dest2, origin1, 0.0f, WHITE);
    }

    if (player->texture.id != 0) {
        DrawTexturePro(player->texture, src1, dest1, origin1, 0.0f, WHITE);
    } else {
        DrawRectangleRec(dest1, player->fliped ? RED : BLUE);
    }
}

void DrawCs2WinCounter(GameState* state) {
    int screen_w = GetScreenWidth();
    int center_x = screen_w / 2;
    int top_y = 16;
    int box_w = 60;
    int box_h = 44;

    // Dark sleek container backing
    DrawRectangleRounded((Rectangle){ center_x - 90, top_y - 4, 180, box_h + 8 }, 0.25f, 4, (Color){ 12, 14, 20, 220 });

    // Player 1 (Blue / CT style) score box
    Rectangle p1_box = { center_x - box_w - 16, top_y, box_w, box_h };
    DrawRectangleRounded(p1_box, 0.2f, 4, (Color){ 24, 76, 150, 240 });
    DrawRectangleRoundedLinesEx(p1_box, 0.2f, 4, 2, (Color){ 75, 145, 245, 255 });

    char p1_str[16];
    snprintf(p1_str, sizeof(p1_str), "%d", state->p1_wins);
    int p1_text_w = MeasureText(p1_str, 28);
    DrawText(p1_str, p1_box.x + (box_w - p1_text_w) / 2, p1_box.y + 8, 28, WHITE);

    // Center divider badge
    Rectangle center_badge = { center_x - 13, top_y + 4, 26, box_h - 8 };
    DrawRectangleRounded(center_badge, 0.2f, 4, (Color){ 20, 22, 30, 255 });
    DrawRectangleRoundedLinesEx(center_badge, 0.2f, 4, 1, (Color){ 65, 70, 85, 255 });
    int vs_w = MeasureText("VS", 14);
    DrawText("VS", center_badge.x + (26 - vs_w) / 2, center_badge.y + 9, 14, (Color){ 185, 190, 210, 255 });

    // Player 2 (Yellow / T style) score box
    Rectangle p2_box = { center_x + 16, top_y, box_w, box_h };
    DrawRectangleRounded(p2_box, 0.2f, 4, (Color){ 180, 135, 18, 240 });
    DrawRectangleRoundedLinesEx(p2_box, 0.2f, 4, 2, (Color){ 250, 200, 45, 255 });

    char p2_str[16];
    snprintf(p2_str, sizeof(p2_str), "%d", state->p2_wins);
    int p2_text_w = MeasureText(p2_str, 28);
    DrawText(p2_str, p2_box.x + (box_w - p2_text_w) / 2, p2_box.y + 8, 28, WHITE);
}

void DrawHealthBars(GameState* state) {
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    int bar_w = 260;
    int bar_h = 24;
    int margin_x = 30;
    int bar_y = screen_h - 45;

    // --- RUST CRAB (BOTTOM LEFT) ---
    int p1_x = margin_x;

    DrawText("Rust Crab", p1_x, bar_y - 22, 16, (Color){ 100, 180, 255, 255 });
    char p1_hp_str[32];
    snprintf(p1_hp_str, sizeof(p1_hp_str), "%d / 100", state->player1.healt);
    int p1_hp_w = MeasureText(p1_hp_str, 16);
    DrawText(p1_hp_str, p1_x + bar_w - p1_hp_w, bar_y - 22, 16, (Color){ 220, 225, 235, 255 });

    // Dark frame behind bar
    DrawRectangle(p1_x - 3, bar_y - 3, bar_w + 6, bar_h + 6, (Color){ 15, 16, 22, 230 });

    // Faint red bar (full width) showing the depleted portion
    DrawRectangle(p1_x, bar_y, bar_w, bar_h, (Color){ 95, 20, 20, 220 });

    // Active health: original bright red
    float p1_pct = (float)state->player1.healt / 100.0f;
    if (p1_pct < 0.0f) p1_pct = 0.0f;
    if (p1_pct > 1.0f) p1_pct = 1.0f;
    int p1_fill_w = (int)(bar_w * p1_pct);

    if (p1_fill_w > 0) {
        DrawRectangle(p1_x, bar_y, p1_fill_w, bar_h, (Color){ 225, 30, 30, 255 });
        // Gloss highlight line at the top
        DrawRectangle(p1_x, bar_y, p1_fill_w, 4, (Color){ 255, 120, 120, 190 });
    }

    // Border outline
    DrawRectangleLinesEx((Rectangle){ p1_x - 2, bar_y - 2, bar_w + 4, bar_h + 4 }, 2, (Color){ 50, 52, 65, 255 });


    // --- C++ CRAB (BOTTOM RIGHT) ---
    int p2_x = screen_w - margin_x - bar_w;

    DrawText("C++ Crab", p2_x, bar_y - 22, 16, (Color){ 255, 215, 65, 255 });
    char p2_hp_str[32];
    snprintf(p2_hp_str, sizeof(p2_hp_str), "%d / 100", state->player2.healt);
    int p2_hp_w = MeasureText(p2_hp_str, 16);
    DrawText(p2_hp_str, p2_x + bar_w - p2_hp_w, bar_y - 22, 16, (Color){ 220, 225, 235, 255 });

    // Dark frame behind bar
    DrawRectangle(p2_x - 3, bar_y - 3, bar_w + 6, bar_h + 6, (Color){ 15, 16, 22, 230 });

    // Faint red bar (full width) showing the depleted portion
    DrawRectangle(p2_x, bar_y, bar_w, bar_h, (Color){ 95, 20, 20, 220 });

    // Active health: original bright red (depletes from center towards right edge)
    float p2_pct = (float)state->player2.healt / 100.0f;
    if (p2_pct < 0.0f) p2_pct = 0.0f;
    if (p2_pct > 1.0f) p2_pct = 1.0f;
    int p2_fill_w = (int)(bar_w * p2_pct);

    if (p2_fill_w > 0) {
        DrawRectangle(p2_x + (bar_w - p2_fill_w), bar_y, p2_fill_w, bar_h, (Color){ 225, 30, 30, 255 });
        DrawRectangle(p2_x + (bar_w - p2_fill_w), bar_y, p2_fill_w, 4, (Color){ 255, 120, 120, 190 });
    }

    // Border outline
    DrawRectangleLinesEx((Rectangle){ p2_x - 2, bar_y - 2, bar_w + 4, bar_h + 4 }, 2, (Color){ 50, 52, 65, 255 });
}

void DrawWinScreen(GameState* state, float subtick_alpha) {
    // Render the arena in the background
    draw_arena(state, subtick_alpha);

    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    // Dim the background
    DrawRectangle(0, 0, screen_w, screen_h, (Color){ 10, 12, 18, 190 });

    // Center victory modal card
    int card_w = 480;
    int card_h = 300;
    int card_x = (screen_w - card_w) / 2;
    int card_y = (screen_h - card_h) / 2;

    Color win_color = (state->winner == 1) ? (Color){ 75, 155, 255, 255 } : (Color){ 255, 210, 55, 255 };

    DrawRectangleRounded((Rectangle){ card_x, card_y, card_w, card_h }, 0.15f, 6, (Color){ 22, 25, 35, 250 });
    DrawRectangleRoundedLinesEx((Rectangle){ card_x, card_y, card_w, card_h }, 0.15f, 6, 3, win_color);

    const char* round_title = "ROUND FINISHED!";
    int title_w = MeasureText(round_title, 32);
    DrawText(round_title, card_x + (card_w - title_w) / 2, card_y + 30, 32, WHITE);

    const char* win_msg = (state->winner == 1) ? "RUST CRAB WINS!" : "C++ CRAB WINS!";
    int msg_w = MeasureText(win_msg, 36);
    DrawText(win_msg, card_x + (card_w - msg_w) / 2, card_y + 80, 36, win_color);

    const char* sub_msg = (state->winner == 1) ? "Rust Crab has defeated C++ Crab!" : "C++ Crab has defeated Rust Crab!";
    int sub_w = MeasureText(sub_msg, 20);
    DrawText(sub_msg, card_x + (card_w - sub_w) / 2, card_y + 130, 20, LIGHTGRAY);

    // Match Score
    char score_str[64];
    snprintf(score_str, sizeof(score_str), "Score:  %d  -  %d", state->p1_wins, state->p2_wins);
    int sc_w = MeasureText(score_str, 24);
    DrawText(score_str, card_x + (card_w - sc_w) / 2, card_y + 175, 24, (Color){ 210, 220, 240, 255 });

    // Respawn countdown & skip hint
    char timer_str[64];
    snprintf(timer_str, sizeof(timer_str), "Respawning in %.1f s...", (state->win_screen_timer > 0.0f) ? state->win_screen_timer : 0.0f);
    int tm_w = MeasureText(timer_str, 18);
    DrawText(timer_str, card_x + (card_w - tm_w) / 2, card_y + 225, 18, (Color){ 165, 175, 190, 255 });

    const char* skip_hint = "Press [SPACE] or [ENTER] to Respawn Now";
    int sk_w = MeasureText(skip_hint, 16);
    DrawText(skip_hint, card_x + (card_w - sk_w) / 2, card_y + 258, 16, (Color){ 125, 135, 150, 255 });
}

void draw_arena(GameState* state, float subtick_alpha) {
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    ClearBackground((Color){ 18, 20, 28, 255 });

    // Subtle arena grid lines
    for (int y = 80; y < (int)STAGE_FLOOR_Y; y += 80) {
        DrawLine(0, y, screen_w, y, (Color){ 26, 30, 42, 120 });
    }
    for (int x = 80; x < screen_w; x += 80) {
        DrawLine(x, 80, x, (int)STAGE_FLOOR_Y, (Color){ 26, 30, 42, 120 });
    }

    // Floor platform: drawn from STAGE_FLOOR_Y to bottom of window
    DrawRectangle(0, (int)STAGE_FLOOR_Y, screen_w, screen_h - (int)STAGE_FLOOR_Y, (Color){ 30, 34, 46, 255 });
    // Floor top edge line on which players stand
    DrawLineEx((Vector2){ 0, STAGE_FLOOR_Y }, (Vector2){ (float)screen_w, STAGE_FLOOR_Y }, 4, (Color){ 75, 85, 115, 255 });
    DrawLineEx((Vector2){ 0, STAGE_FLOOR_Y + 4 }, (Vector2){ (float)screen_w, STAGE_FLOOR_Y + 4 }, 2, (Color){ 105, 120, 155, 140 });

    // Draw the two fighters
    DrawPlayer(&state->player1, subtick_alpha);
    DrawPlayer(&state->player2, subtick_alpha);

    // CS2-style Win Counter (Top Center)
    DrawCs2WinCounter(state);

    // Health Bars (Bottom Left and Right)
    DrawHealthBars(state);

    // Navigation hint
    DrawText("[ESC] Menu", 16, 16, 14, (Color){ 120, 130, 150, 180 });
}

void DrawGame(GameState* state, float subtick_alpha) {
    BeginDrawing();
    ClearBackground(WALAHI_COLOUR);

    switch (state->current_page) {
        case SAY_WALAHI:
            draw_walahi(state, subtick_alpha);
            break;
        case HOME:
            state->buttons.rect[0] = (Rectangle){ 260, 260, 280, 56 };
            state->buttons.rect[1] = (Rectangle){ 260, 335, 280, 56 };
            draw_home(state, subtick_alpha);
            break;
        case ARENA:
            draw_arena(state, subtick_alpha);
            break;
        case WIN_SCREEN:
            DrawWinScreen(state, subtick_alpha);
            break;
    }

    EndDrawing();
}
