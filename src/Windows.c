//
// Created by timci on 24.08.26.
//

#include "include/Windows.h"

#include <stdio.h>

#include "raylib.h"
#include "include/ButtonControls.h"

void MainMenu() {
    Rectangle button1 = (Rectangle){10,10,100,30};
    BeginDrawing();
    DrawRectangle(button1.x,button1.y,button1.width,button1.height,RAYWHITE);
    ClearBackground(BLACK);
    EndDrawing();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        switch (checkMouseClick(1,GetMousePosition(),button1)) {
            case 0:
                printf(("fuck u"));
                break;
                default:
                printf("NEOEEEEEEE");
                break;
        }
    }

}