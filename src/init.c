//
// Created by timci on 24.08.26.
//

#include "include/init.h"
#include "raylib.h"
#include <stdio.h>

void init() {
    InitWindow(800, 800, "Turgau Fighter");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
}

Texture2D LoadGameTexture(const char* filename) {
    char path[512];
    snprintf(path, sizeof(path), "assets/%s", filename);
    if (FileExists(path)) {
        return LoadTexture(path);
    }
    snprintf(path, sizeof(path), "../assets/%s", filename);
    if (FileExists(path)) {
        return LoadTexture(path);
    }
    snprintf(path, sizeof(path), "../../assets/%s", filename);
    if (FileExists(path)) {
        return LoadTexture(path);
    }
    return LoadTexture(filename);
}