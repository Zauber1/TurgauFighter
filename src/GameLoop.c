//
// Created by timci on 24.08.26.
//

#include "include/GameLoop.h"
#include "raylib.h"
#include "include/Windows.h"
void GameLoop() {
    while (!WindowShouldClose()) {
        MainMenu();
    }
}