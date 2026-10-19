#ifndef TURGAUFIGHTER_SHADERS_H
#define TURGAUFIGHTER_SHADERS_H

#include "raylib.h"

typedef enum CrabShader {
    SHADER_NONE = 0,
    SHADER_GLINT = 1,       // Minecraft Enchanted Glint
    SHADER_MANDELBROT = 2,  // Mandelbrot Fractal
    SHADER_HOLO = 3,        // CS2 Rainbow Holographic
    SHADER_GOLD = 4,        // Gold Foil
    SHADER_FIRE = 5,        // Fire Magma Aura
    SHADER_COUNT = 6
} CrabShader;

void InitShaders(void);
void UnloadAllShaders(void);
void ApplyCrabShader(int shader_id, float time);
void EndCrabShader(void);
void ApplyHorizontalScanShader(float time);
void EndHorizontalScanShader(void);

const char* GetShaderName(int shader_id);
Color GetShaderRarityColor(int shader_id);
const char* GetShaderRarityName(int shader_id);

void InitClickSound(void);
void PlayReelClickSound(void);

#endif // TURGAUFIGHTER_SHADERS_H
