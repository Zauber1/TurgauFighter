#define WALAHI_COLOUR CLITERAL(Color){ 0, 0, 0, 255 }
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "include/ButtonControls.h"
#include "include/Windows.h"
#include "include/logic.h"
#include "include/init.h"
#include "include/shaders.h"

void draw_arena(GameState* state, float subtick_alpha);
void draw_gambling(GameState* state, float subtick_alpha);

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
    int title_size = 56;
    int title_w = MeasureText(title, title_size);
    int title_x = (screen_w - title_w) / 2;
    int title_y = 75;

    DrawText(title, title_x + 3, title_y + 3, title_size, (Color){ 5, 5, 10, 240 });
    DrawText(title, title_x, title_y, title_size, (Color){ 245, 190, 45, 255 });

    const char* subtitle = "Rust Crab vs C++ Crab -- Battle for Memory Safety";
    int sub_w = MeasureText(subtitle, 18);
    DrawText(subtitle, (screen_w - sub_w) / 2, title_y + 65, 18, (Color){ 175, 185, 205, 255 });

    // Character preview artwork with equipped shaders
    if (state->player1.texture.id != 0) {
        float r1 = (float)state->player1.texture.width / (float)state->player1.texture.height;
        if (state->player1.shader_id > 0) {
            ApplyCrabShader(state->player1.shader_id, state->run_time);
        }
        DrawTexturePro(state->player1.texture,
            (Rectangle){ 0.0f, 0.0f, (float)state->player1.texture.width, (float)state->player1.texture.height },
            (Rectangle){ 55.0f, 230.0f, 135.0f * r1, 135.0f },
            (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        if (state->player1.shader_id > 0) {
            EndCrabShader();
        }
        int p1_lbl_w = MeasureText("Rust Crab", 18);
        DrawText("Rust Crab", 55 + (int)(135.0f * r1 - p1_lbl_w) / 2, 380, 18, (Color){ 100, 175, 255, 255 });
        const char* s1_name = GetShaderName(state->player1.shader_id);
        int s1_w = MeasureText(s1_name, 14);
        DrawText(s1_name, 55 + (int)(135.0f * r1 - s1_w) / 2, 405, 14, GetShaderRarityColor(state->player1.shader_id));
    }

    if (state->player2.texture.id != 0) {
        float r2 = (float)state->player2.texture.width / (float)state->player2.texture.height;
        float p2_drawn_w = 135.0f * r2;
        if (state->player2.shader_id > 0) {
            ApplyCrabShader(state->player2.shader_id, state->run_time);
        }
        DrawTexturePro(state->player2.texture,
            (Rectangle){ 0.0f, 0.0f, -(float)state->player2.texture.width, (float)state->player2.texture.height },
            (Rectangle){ (float)screen_w - 55.0f - p2_drawn_w, 230.0f, p2_drawn_w, 135.0f },
            (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        if (state->player2.shader_id > 0) {
            EndCrabShader();
        }
        int p2_lbl_w = MeasureText("C++ Crab", 18);
        DrawText("C++ Crab", screen_w - 55 - (int)p2_drawn_w + (int)(p2_drawn_w - p2_lbl_w) / 2, 380, 18, (Color){ 255, 210, 65, 255 });
        const char* s2_name = GetShaderName(state->player2.shader_id);
        int s2_w = MeasureText(s2_name, 14);
        DrawText(s2_name, screen_w - 55 - (int)p2_drawn_w + (int)(p2_drawn_w - s2_w) / 2, 405, 14, GetShaderRarityColor(state->player2.shader_id));
    }

    // Home buttons: PLAY ARENA, OPEN CASES (GAMBLE), SAY WALAHI
    Rectangle btn_arena = state->buttons.rect[0];
    Rectangle btn_gamble = state->buttons.rect[1];
    Rectangle btn_walahi = state->buttons.rect[2];

    Vector2 mouse = GetMousePosition();
    bool hover1 = CheckCollisionPointRec(mouse, btn_arena);
    bool hover2 = CheckCollisionPointRec(mouse, btn_gamble);
    bool hover3 = CheckCollisionPointRec(mouse, btn_walahi);

    // Button 1: PLAY ARENA
    Color b1_bg = hover1 ? (Color){ 45, 95, 205, 255 } : (Color){ 28, 55, 125, 245 };
    Color b1_border = hover1 ? (Color){ 130, 195, 255, 255 } : (Color){ 65, 125, 230, 255 };
    DrawRectangleRounded(btn_arena, 0.25f, 4, b1_bg);
    DrawRectangleRoundedLinesEx(btn_arena, 0.25f, 4, 2, b1_border);
    const char* b1_text = "PLAY ARENA";
    int b1_w = MeasureText(b1_text, 22);
    DrawText(b1_text, btn_arena.x + (btn_arena.width - b1_w) / 2, btn_arena.y + 15, 22, WHITE);

    // Button 2: OPEN CASES (GAMBLE)
    Color b2_bg = hover2 ? (Color){ 180, 45, 140, 255 } : (Color){ 115, 25, 90, 245 };
    Color b2_border = hover2 ? (Color){ 255, 120, 215, 255 } : (Color){ 200, 55, 155, 255 };
    DrawRectangleRounded(btn_gamble, 0.25f, 4, b2_bg);
    DrawRectangleRoundedLinesEx(btn_gamble, 0.25f, 4, 2, b2_border);
    const char* b2_text = "OPEN CASES (GAMBLE)";
    int b2_w = MeasureText(b2_text, 20);
    DrawText(b2_text, btn_gamble.x + (btn_gamble.width - b2_w) / 2, btn_gamble.y + 16, 20, WHITE);

    // Button 3: SAY WALAHI
    Color b3_bg = hover3 ? (Color){ 175, 130, 20, 255 } : (Color){ 110, 80, 15, 245 };
    Color b3_border = hover3 ? (Color){ 255, 225, 80, 255 } : (Color){ 210, 165, 35, 255 };
    DrawRectangleRounded(btn_walahi, 0.25f, 4, b3_bg);
    DrawRectangleRoundedLinesEx(btn_walahi, 0.25f, 4, 2, b3_border);
    const char* b3_text = "SAY WALAHI";
    int b3_w = MeasureText(b3_text, 22);
    DrawText(b3_text, btn_walahi.x + (btn_walahi.width - b3_w) / 2, btn_walahi.y + 15, 22, WHITE);

    // Footer decoration
    DrawText("Turgau Fighter v1.337", (screen_w - MeasureText("Turgau Fighter v1.337", 14)) / 2, screen_h - 45, 14, (Color){ 90, 95, 120, 200 });

    // Handle mouse click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int clicked = checkMouseClick(3, mouse, btn_arena, btn_gamble, btn_walahi);
        switch (clicked) {
            case 0:
                reset_round(state);
                state->current_page = ARENA;
                break;
            case 1:
                state->current_page = GAMBLING;
                break;
            case 2:
                state->current_page = SAY_WALAHI;
                break;
            default:
                break;
        }
    }
}

void DrawPlayer(PlayerState* player, float subtick_alpha, float time) {
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
        // Apply player's equipped shader effect
        if (player->shader_id > 0) {
            ApplyCrabShader(player->shader_id, time);
        }
        DrawTexturePro(player->texture, src1, dest1, origin1, 0.0f, WHITE);
        if (player->shader_id > 0) {
            EndCrabShader();
        }
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

    // Blue / CT style score box
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

    // Yellow / T style score box
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

    // Draw the two fighters standing on the floor
    DrawPlayer(&state->player1, subtick_alpha, state->run_time);
    DrawPlayer(&state->player2, subtick_alpha, state->run_time);

    // CS2-style Win Counter (Top Center)
    DrawCs2WinCounter(state);

    // Health Bars (Bottom Left and Right)
    DrawHealthBars(state);

    // Navigation hint
    DrawText("[ESC] Menu", 16, 16, 14, (Color){ 120, 130, 150, 180 });
}

void draw_gambling(GameState* state, float subtick_alpha) {
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();

    ClearBackground((Color){ 14, 16, 24, 255 });

    // Apply horizontal sweeping holographic scan shader across screen
    ApplyHorizontalScanShader(state->run_time);
    DrawRectangle(0, 0, screen_w, screen_h, (Color){ 20, 24, 40, 130 });
    EndHorizontalScanShader();

    // Top Header
    const char* hdr = "CASE OPENING & SHADER GAMBLE";
    int hw = MeasureText(hdr, 28);
    DrawText(hdr, (screen_w - hw) / 2, 20, 28, (Color){ 250, 200, 45, 255 });
    const char* sub = "Spin for free to unbox rare shaders and equip them to your crabs!";
    int sw = MeasureText(sub, 15);
    DrawText(sub, (screen_w - sw) / 2, 54, 15, (Color){ 175, 185, 205, 255 });

    // Navigation hint
    DrawText("[ESC] Menu", 16, 16, 14, (Color){ 130, 140, 165, 200 });

    // --- HORIZONTAL CS2-STYLE ROULETTE REEL ---
    int reel_x = 20;
    int reel_y = 80;
    int reel_w = screen_w - 40;
    int reel_h = 145;

    // Outer frame of the reel
    DrawRectangleRounded((Rectangle){ reel_x - 3, reel_y - 3, reel_w + 6, reel_h + 6 }, 0.1f, 4, (Color){ 10, 12, 18, 240 });
    DrawRectangleRounded((Rectangle){ reel_x, reel_y, reel_w, reel_h }, 0.1f, 4, (Color){ 22, 25, 36, 255 });
    DrawRectangleRoundedLinesEx((Rectangle){ reel_x, reel_y, reel_w, reel_h }, 0.1f, 4, 2, (Color){ 60, 68, 95, 255 });

    // Scissor mode to cleanly clip items inside reel
    BeginScissorMode(reel_x, reel_y, reel_w, reel_h);

    int card_w = 125;
    int card_h = 125;
    int card_step = 140;

    for (int i = 0; i < 64; i++) {
        float cx = 400.0f + (float)i * card_step - state->gambling.reel_pos - (card_w / 2.0f);
        if (cx + card_w < reel_x || cx > reel_x + reel_w) {
            continue; // Skip off-screen cards
        }

        int s_id = state->gambling.reel_items[i];
        Color rCol = GetShaderRarityColor(s_id);

        Rectangle card_rec = { cx, reel_y + 10, card_w, card_h };
        DrawRectangleRounded(card_rec, 0.12f, 4, (Color){ 30, 34, 48, 255 });
        DrawRectangleRoundedLinesEx(card_rec, 0.12f, 4, 2, rCol);

        // Bottom rarity stripe
        DrawRectangleRounded((Rectangle){ cx + 2, reel_y + card_h - 10, card_w - 4, 18 }, 0.2f, 4, rCol);

        // Card icon: preview ferris with shader
        if (state->player1.texture.id != 0) {
            float r = (float)state->player1.texture.width / (float)state->player1.texture.height;
            float icon_h = 60.0f;
            float icon_w = icon_h * r;
            Rectangle icon_dest = { cx + (card_w - icon_w) / 2.0f, reel_y + 18, icon_w, icon_h };

            if (s_id > 0) {
                ApplyCrabShader(s_id, state->run_time);
            }
            DrawTexturePro(state->player1.texture,
                (Rectangle){ 0, 0, (float)state->player1.texture.width, (float)state->player1.texture.height },
                icon_dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
            if (s_id > 0) {
                EndCrabShader();
            }
        }

        // Shader name
        const char* sname = GetShaderName(s_id);
        int snw = MeasureText(sname, 12);
        DrawText(sname, cx + (card_w - snw) / 2, reel_y + 84, 12, WHITE);

        // Rarity text
        const char* rname = GetShaderRarityName(s_id);
        int rnw = MeasureText(rname, 11);
        DrawText(rname, cx + (card_w - rnw) / 2, reel_y + 115, 11, (Color){ 15, 15, 20, 255 });
    }

    EndScissorMode();

    // Center Selection Indicator (CS2 Gold Needle)
    float needle_x = 400.0f;
    DrawLineEx((Vector2){ needle_x, reel_y }, (Vector2){ needle_x, reel_y + reel_h }, 3, (Color){ 255, 215, 50, 240 });
    // Top arrow
    DrawTriangle((Vector2){ needle_x - 9, reel_y }, (Vector2){ needle_x + 9, reel_y }, (Vector2){ needle_x, reel_y + 16 }, (Color){ 255, 215, 50, 255 });
    // Bottom arrow
    DrawTriangle((Vector2){ needle_x, reel_y + reel_h - 16 }, (Vector2){ needle_x + 9, reel_y + reel_h }, (Vector2){ needle_x - 9, reel_y + reel_h }, (Color){ 255, 215, 50, 255 });

    // --- SPIN BUTTON / WIN BANNER ---
    Vector2 mouse = GetMousePosition();
    Rectangle spin_btn = { (screen_w - 220) / 2, reel_y + reel_h + 16, 220, 46 };
    bool hover_spin = CheckCollisionPointRec(mouse, spin_btn);

    if (state->gambling.is_spinning) {
        DrawRectangleRounded(spin_btn, 0.25f, 4, (Color){ 60, 65, 80, 220 });
        DrawRectangleRoundedLinesEx(spin_btn, 0.25f, 4, 2, (Color){ 100, 110, 135, 255 });
        const char* sp_txt = "OPENING...";
        int sp_w = MeasureText(sp_txt, 20);
        DrawText(sp_txt, spin_btn.x + (spin_btn.width - sp_w) / 2, spin_btn.y + 13, 20, LIGHTGRAY);
    } else {
        Color s_bg = hover_spin ? (Color){ 45, 165, 85, 255 } : (Color){ 30, 125, 60, 240 };
        Color s_border = hover_spin ? (Color){ 100, 240, 145, 255 } : (Color){ 55, 185, 95, 255 };
        DrawRectangleRounded(spin_btn, 0.25f, 4, s_bg);
        DrawRectangleRoundedLinesEx(spin_btn, 0.25f, 4, 2, s_border);
        const char* sp_txt = "SPIN (FREE)";
        int sp_w = MeasureText(sp_txt, 20);
        DrawText(sp_txt, spin_btn.x + (spin_btn.width - sp_w) / 2, spin_btn.y + 13, 20, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hover_spin) {
            start_gambling_spin(state);
        }
    }

    // Unbox result banner
    if (state->gambling.has_won && !state->gambling.is_spinning) {
        int w_id = state->gambling.winning_shader;
        Color win_col = GetShaderRarityColor(w_id);

        Rectangle win_card = { (screen_w - 560) / 2, reel_y + reel_h + 68, 560, 72 };
        DrawRectangleRounded(win_card, 0.15f, 4, (Color){ 20, 24, 38, 250 });
        DrawRectangleRoundedLinesEx(win_card, 0.15f, 4, 2, win_col);

        char unbox_txt[128];
        snprintf(unbox_txt, sizeof(unbox_txt), "UNBOXED: %s (%s)!", GetShaderName(w_id), GetShaderRarityName(w_id));
        int ut_w = MeasureText(unbox_txt, 16);
        DrawText(unbox_txt, win_card.x + (win_card.width - ut_w) / 2, win_card.y + 6, 16, win_col);

        const char* choice_prompt = "Select who gets this shader (only that character receives it):";
        int cp_w = MeasureText(choice_prompt, 12);
        DrawText(choice_prompt, win_card.x + (win_card.width - cp_w) / 2, win_card.y + 25, 12, (Color){ 180, 190, 210, 255 });

        // Distinct choice buttons: Give to Rust Crab OR Give to C++ Crab
        Rectangle eq1_btn = { win_card.x + 20, win_card.y + 42, 245, 22 };
        Rectangle eq2_btn = { win_card.x + 295, win_card.y + 42, 245, 22 };
        bool h_eq1 = CheckCollisionPointRec(mouse, eq1_btn);
        bool h_eq2 = CheckCollisionPointRec(mouse, eq2_btn);

        DrawRectangleRounded(eq1_btn, 0.2f, 4, h_eq1 ? (Color){ 45, 95, 215, 255 } : (Color){ 25, 55, 125, 240 });
        DrawRectangleRoundedLinesEx(eq1_btn, 0.2f, 4, 1, (Color){ 80, 155, 255, 255 });
        const char* g1_txt = "Give to Rust Crab";
        int g1_w = MeasureText(g1_txt, 13);
        DrawText(g1_txt, eq1_btn.x + (eq1_btn.width - g1_w) / 2, eq1_btn.y + 4, 13, WHITE);

        DrawRectangleRounded(eq2_btn, 0.2f, 4, h_eq2 ? (Color){ 195, 145, 25, 255 } : (Color){ 115, 80, 15, 240 });
        DrawRectangleRoundedLinesEx(eq2_btn, 0.2f, 4, 1, (Color){ 255, 205, 55, 255 });
        const char* g2_txt = "Give to C++ Crab";
        int g2_w = MeasureText(g2_txt, 13);
        DrawText(g2_txt, eq2_btn.x + (eq2_btn.width - g2_w) / 2, eq2_btn.y + 4, 13, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (h_eq1) {
                state->player1.owned_shaders[w_id] = true;
                state->player1.shader_id = w_id;
                state->gambling.has_won = false;
            } else if (h_eq2) {
                state->player2.owned_shaders[w_id] = true;
                state->player2.shader_id = w_id;
                state->gambling.has_won = false;
            }
        }
    }

    // --- SHADER SELECTOR & AVATAR PREVIEW PANELS ---
    int panel_y = 425;
    int panel_w = 360;
    int panel_h = 350;

    // LEFT: Rust Crab (Blue)
    int p1_panel_x = 25;
    DrawRectangleRounded((Rectangle){ p1_panel_x, panel_y, panel_w, panel_h }, 0.1f, 4, (Color){ 20, 24, 36, 240 });
    DrawRectangleRoundedLinesEx((Rectangle){ p1_panel_x, panel_y, panel_w, panel_h }, 0.1f, 4, 2, (Color){ 65, 125, 230, 255 });

    DrawText("RUST CRAB SHADER", p1_panel_x + 20, panel_y + 14, 18, (Color){ 100, 180, 255, 255 });

    // Live Avatar Preview for Rust Crab
    if (state->player1.texture.id != 0) {
        float r1 = (float)state->player1.texture.width / (float)state->player1.texture.height;
        float prev_h = 90.0f;
        float prev_w = prev_h * r1;
        Rectangle prev_rec = { p1_panel_x + (panel_w - prev_w) / 2.0f, panel_y + 40, prev_w, prev_h };

        if (state->player1.shader_id > 0) {
            ApplyCrabShader(state->player1.shader_id, state->run_time);
        }
        DrawTexturePro(state->player1.texture,
            (Rectangle){ 0, 0, (float)state->player1.texture.width, (float)state->player1.texture.height },
            prev_rec, (Vector2){ 0, 0 }, 0.0f, WHITE);
        if (state->player1.shader_id > 0) {
            EndCrabShader();
        }
    }

    const char* curr_s1 = GetShaderName(state->player1.shader_id);
    char eq_lbl1[64];
    snprintf(eq_lbl1, sizeof(eq_lbl1), "Active: %s", curr_s1);
    int e1_w = MeasureText(eq_lbl1, 14);
    DrawText(eq_lbl1, p1_panel_x + (panel_w - e1_w) / 2, panel_y + 135, 14, GetShaderRarityColor(state->player1.shader_id));

    // Shader buttons for Rust Crab (2 columns of 3 buttons)
    int btn_w = 160;
    int btn_h = 36;
    for (int s = 0; s < SHADER_COUNT; s++) {
        int col = s % 2;
        int row = s / 2;
        int bx = p1_panel_x + 15 + col * 170;
        int by = panel_y + 160 + row * 44;
        Rectangle b_rec = { bx, by, btn_w, btn_h };
        bool h_btn = CheckCollisionPointRec(mouse, b_rec);
        bool is_owned = state->player1.owned_shaders[s];
        bool is_active = (state->player1.shader_id == s);

        if (is_owned) {
            Color bg = is_active ? (Color){ 45, 95, 210, 255 } : (h_btn ? (Color){ 40, 48, 68, 255 } : (Color){ 28, 32, 45, 255 });
            Color border = is_active ? (Color){ 120, 195, 255, 255 } : GetShaderRarityColor(s);

            DrawRectangleRounded(b_rec, 0.2f, 4, bg);
            DrawRectangleRoundedLinesEx(b_rec, 0.2f, 4, is_active ? 2 : 1, border);

            const char* s_name = GetShaderName(s);
            int tw = MeasureText(s_name, 13);
            DrawText(s_name, bx + (btn_w - tw) / 2, by + 11, 13, is_active ? WHITE : (Color){ 210, 215, 230, 255 });

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && h_btn) {
                state->player1.shader_id = s;
            }
        } else {
            // Locked
            DrawRectangleRounded(b_rec, 0.2f, 4, (Color){ 16, 18, 25, 220 });
            DrawRectangleRoundedLinesEx(b_rec, 0.2f, 4, 1, (Color){ 38, 42, 55, 200 });

            const char* s_name = GetShaderName(s);
            int tw = MeasureText(s_name, 11);
            DrawText(s_name, bx + (btn_w - tw) / 2, by + 4, 11, (Color){ 110, 115, 130, 220 });

            const char* lock_txt = "[Locked]";
            int ltw = MeasureText(lock_txt, 11);
            DrawText(lock_txt, bx + (btn_w - ltw) / 2, by + 18, 11, (Color){ 160, 75, 75, 220 });
        }
    }

    // RIGHT: C++ Crab (Yellow)
    int p2_panel_x = screen_w - panel_w - 25;
    DrawRectangleRounded((Rectangle){ p2_panel_x, panel_y, panel_w, panel_h }, 0.1f, 4, (Color){ 24, 22, 20, 240 });
    DrawRectangleRoundedLinesEx((Rectangle){ p2_panel_x, panel_y, panel_w, panel_h }, 0.1f, 4, 2, (Color){ 245, 195, 45, 255 });

    DrawText("C++ CRAB SHADER", p2_panel_x + 20, panel_y + 14, 18, (Color){ 255, 215, 65, 255 });

    // Live Avatar Preview for C++ Crab
    if (state->player2.texture.id != 0) {
        float r2 = (float)state->player2.texture.width / (float)state->player2.texture.height;
        float prev_h = 90.0f;
        float prev_w = prev_h * r2;
        Rectangle prev_rec = { p2_panel_x + (panel_w - prev_w) / 2.0f, panel_y + 40, prev_w, prev_h };

        if (state->player2.shader_id > 0) {
            ApplyCrabShader(state->player2.shader_id, state->run_time);
        }
        DrawTexturePro(state->player2.texture,
            (Rectangle){ 0, 0, -(float)state->player2.texture.width, (float)state->player2.texture.height },
            prev_rec, (Vector2){ 0, 0 }, 0.0f, WHITE);
        if (state->player2.shader_id > 0) {
            EndCrabShader();
        }
    }

    const char* curr_s2 = GetShaderName(state->player2.shader_id);
    char eq_lbl2[64];
    snprintf(eq_lbl2, sizeof(eq_lbl2), "Active: %s", curr_s2);
    int e2_w = MeasureText(eq_lbl2, 14);
    DrawText(eq_lbl2, p2_panel_x + (panel_w - e2_w) / 2, panel_y + 135, 14, GetShaderRarityColor(state->player2.shader_id));

    // Shader buttons for C++ Crab (2 columns of 3 buttons)
    for (int s = 0; s < SHADER_COUNT; s++) {
        int col = s % 2;
        int row = s / 2;
        int bx = p2_panel_x + 15 + col * 170;
        int by = panel_y + 160 + row * 44;
        Rectangle b_rec = { bx, by, btn_w, btn_h };
        bool h_btn = CheckCollisionPointRec(mouse, b_rec);
        bool is_owned = state->player2.owned_shaders[s];
        bool is_active = (state->player2.shader_id == s);

        if (is_owned) {
            Color bg = is_active ? (Color){ 195, 145, 25, 255 } : (h_btn ? (Color){ 55, 48, 30, 255 } : (Color){ 35, 30, 22, 255 });
            Color border = is_active ? (Color){ 255, 225, 80, 255 } : GetShaderRarityColor(s);

            DrawRectangleRounded(b_rec, 0.2f, 4, bg);
            DrawRectangleRoundedLinesEx(b_rec, 0.2f, 4, is_active ? 2 : 1, border);

            const char* s_name = GetShaderName(s);
            int tw = MeasureText(s_name, 13);
            DrawText(s_name, bx + (btn_w - tw) / 2, by + 11, 13, is_active ? WHITE : (Color){ 230, 220, 205, 255 });

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && h_btn) {
                state->player2.shader_id = s;
            }
        } else {
            // Locked
            DrawRectangleRounded(b_rec, 0.2f, 4, (Color){ 20, 18, 16, 220 });
            DrawRectangleRoundedLinesEx(b_rec, 0.2f, 4, 1, (Color){ 45, 40, 35, 200 });

            const char* s_name = GetShaderName(s);
            int tw = MeasureText(s_name, 11);
            DrawText(s_name, bx + (btn_w - tw) / 2, by + 4, 11, (Color){ 120, 115, 105, 220 });

            const char* lock_txt = "[Locked]";
            int ltw = MeasureText(lock_txt, 11);
            DrawText(lock_txt, bx + (btn_w - ltw) / 2, by + 18, 11, (Color){ 160, 75, 75, 220 });
        }
    }
}

void DrawGame(GameState* state, float subtick_alpha) {
    BeginDrawing();
    ClearBackground(WALAHI_COLOUR);

    switch (state->current_page) {
        case SAY_WALAHI:
            draw_walahi(state, subtick_alpha);
            break;
        case HOME:
            state->buttons.num = 3;
            state->buttons.rect[0] = (Rectangle){ 260, 240, 280, 52 };
            state->buttons.rect[1] = (Rectangle){ 260, 310, 280, 52 };
            state->buttons.rect[2] = (Rectangle){ 260, 380, 280, 52 };
            draw_home(state, subtick_alpha);
            break;
        case ARENA:
            draw_arena(state, subtick_alpha);
            break;
        case WIN_SCREEN:
            DrawWinScreen(state, subtick_alpha);
            break;
        case GAMBLING:
            draw_gambling(state, subtick_alpha);
            break;
    }

    EndDrawing();
}
