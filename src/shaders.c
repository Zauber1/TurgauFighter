#include "include/shaders.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static Shader crab_shaders[SHADER_COUNT] = { 0 };
static int time_locs[SHADER_COUNT] = { -1 };

static Shader scan_shader = { 0 };
static int scan_time_loc = -1;

static Sound click_sound = { 0 };
static bool sound_initialized = false;

// 1. Minecraft Enchanted Glint: Purple/magenta shimmer moving diagonally
static const char* GLINT_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    if (base.a < 0.05) { finalColor = base; return; }\n"
    "    float p1 = sin((fragTexCoord.x + fragTexCoord.y) * 14.0 - uTime * 4.5);\n"
    "    float p2 = sin((fragTexCoord.x * 1.5 - fragTexCoord.y) * 18.0 + uTime * 3.5);\n"
    "    float glint = clamp((p1 + p2) * 0.5 + 0.35, 0.0, 1.0);\n"
    "    vec3 glintColor = vec3(0.72, 0.22, 1.0) * glint * 0.75;\n"
    "    finalColor = vec4(base.rgb + glintColor * base.a, base.a);\n"
    "}\n";

// 2. Mandelbrot Fractal shader
static const char* MANDELBROT_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    if (base.a < 0.05) { finalColor = base; return; }\n"
    "    float zoom = 1.3 + 0.35 * sin(uTime * 0.7);\n"
    "    vec2 c = (fragTexCoord - vec2(0.5, 0.5)) * zoom + vec2(-0.745, 0.13);\n"
    "    vec2 z = c;\n"
    "    int iter = 0;\n"
    "    for (int i = 0; i < 32; i++) {\n"
    "        if (dot(z, z) > 4.0) break;\n"
    "        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;\n"
    "        iter++;\n"
    "    }\n"
    "    float t = float(iter) / 32.0;\n"
    "    vec3 fractal = 0.5 + 0.5 * cos(6.28318 * (vec3(1.0, 1.0, 1.0) * t + vec3(0.0, 0.33, 0.67) + uTime * 0.4));\n"
    "    finalColor = vec4(mix(base.rgb, fractal, 0.68), base.a);\n"
    "}\n";

// 3. CS2 Rainbow Holographic Sticker Shader
static const char* HOLO_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    if (base.a < 0.05) { finalColor = base; return; }\n"
    "    float wave = fragTexCoord.x * 3.0 + fragTexCoord.y * 2.0 + uTime * 2.5;\n"
    "    vec3 rainbow = 0.5 + 0.5 * cos(wave + vec3(0.0, 2.094, 4.188));\n"
    "    float shine = pow(max(0.0, sin((fragTexCoord.x * 4.0 - fragTexCoord.y * 3.0) + uTime * 3.0)), 6.0);\n"
    "    vec3 result = mix(base.rgb, rainbow, 0.45) + vec3(shine * 0.55);\n"
    "    finalColor = vec4(result, base.a);\n"
    "}\n";

// 4. CS2 Gold Foil Shader
static const char* GOLD_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    if (base.a < 0.05) { finalColor = base; return; }\n"
    "    float sheen = pow(max(0.0, sin((fragTexCoord.x + fragTexCoord.y * 0.6) * 10.0 - uTime * 3.8)), 4.0);\n"
    "    vec3 goldDark = vec3(0.70, 0.50, 0.10);\n"
    "    vec3 goldBright = vec3(1.0, 0.90, 0.40);\n"
    "    vec3 gold = mix(goldDark, goldBright, sheen);\n"
    "    float lum = dot(base.rgb, vec3(0.299, 0.587, 0.114));\n"
    "    vec3 result = gold * (lum * 1.3 + 0.2) + vec3(sheen * 0.45);\n"
    "    finalColor = vec4(result, base.a);\n"
    "}\n";

// 5. Fire Magma Aura Shader
static const char* FIRE_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    if (base.a < 0.05) { finalColor = base; return; }\n"
    "    float f1 = sin(fragTexCoord.y * 24.0 - uTime * 7.0) * 0.5 + 0.5;\n"
    "    float f2 = cos(fragTexCoord.x * 20.0 + uTime * 5.0) * 0.5 + 0.5;\n"
    "    float flame = clamp((f1 * 0.7 + f2 * 0.3) * (1.2 - fragTexCoord.y * 0.5), 0.0, 1.0);\n"
    "    vec3 fireColor = mix(vec3(1.0, 0.15, 0.0), vec3(1.0, 0.85, 0.1), flame);\n"
    "    vec3 result = mix(base.rgb, fireColor, 0.58) + vec3(flame * 0.25);\n"
    "    finalColor = vec4(result, base.a);\n"
    "}\n";

// Horizontal Screen Scanner Shader
static const char* SCAN_FS = 
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "in vec4 fragColor;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D texture0;\n"
    "uniform vec4 colDiffuse;\n"
    "uniform float uTime;\n"
    "void main()\n"
    "{\n"
    "    vec4 base = texture(texture0, fragTexCoord) * colDiffuse * fragColor;\n"
    "    float sweep = mod(uTime * 0.35, 1.4) - 0.2;\n"
    "    float dist = abs(fragTexCoord.x - sweep);\n"
    "    float beam = exp(-dist * dist * 140.0);\n"
    "    float lines = sin(fragTexCoord.y * 220.0) * 0.05;\n"
    "    vec3 beamColor = vec3(0.25, 0.75, 1.0) * beam * 0.45;\n"
    "    finalColor = vec4(base.rgb + beamColor + lines, base.a);\n"
    "}\n";

void InitShaders(void) {
    crab_shaders[SHADER_NONE] = (Shader){ 0 };
    crab_shaders[SHADER_GLINT] = LoadShaderFromMemory(NULL, GLINT_FS);
    crab_shaders[SHADER_MANDELBROT] = LoadShaderFromMemory(NULL, MANDELBROT_FS);
    crab_shaders[SHADER_HOLO] = LoadShaderFromMemory(NULL, HOLO_FS);
    crab_shaders[SHADER_GOLD] = LoadShaderFromMemory(NULL, GOLD_FS);
    crab_shaders[SHADER_FIRE] = LoadShaderFromMemory(NULL, FIRE_FS);

    for (int i = 1; i < SHADER_COUNT; i++) {
        if (crab_shaders[i].id != 0) {
            time_locs[i] = GetShaderLocation(crab_shaders[i], "uTime");
        } else {
            time_locs[i] = -1;
        }
    }

    scan_shader = LoadShaderFromMemory(NULL, SCAN_FS);
    if (scan_shader.id != 0) {
        scan_time_loc = GetShaderLocation(scan_shader, "uTime");
    }

    InitClickSound();
}

void UnloadAllShaders(void) {
    for (int i = 1; i < SHADER_COUNT; i++) {
        if (crab_shaders[i].id != 0) {
            UnloadShader(crab_shaders[i]);
            crab_shaders[i] = (Shader){ 0 };
        }
    }
    if (scan_shader.id != 0) {
        UnloadShader(scan_shader);
        scan_shader = (Shader){ 0 };
    }
    if (sound_initialized) {
        UnloadSound(click_sound);
        sound_initialized = false;
    }
}

void ApplyCrabShader(int shader_id, float time) {
    if (shader_id > 0 && shader_id < SHADER_COUNT && crab_shaders[shader_id].id != 0) {
        if (time_locs[shader_id] != -1) {
            SetShaderValue(crab_shaders[shader_id], time_locs[shader_id], &time, SHADER_UNIFORM_FLOAT);
        }
        BeginShaderMode(crab_shaders[shader_id]);
    }
}

void EndCrabShader(void) {
    EndShaderMode();
}

void ApplyHorizontalScanShader(float time) {
    if (scan_shader.id != 0) {
        if (scan_time_loc != -1) {
            SetShaderValue(scan_shader, scan_time_loc, &time, SHADER_UNIFORM_FLOAT);
        }
        BeginShaderMode(scan_shader);
    }
}

void EndHorizontalScanShader(void) {
    EndShaderMode();
}

const char* GetShaderName(int shader_id) {
    switch (shader_id) {
        case SHADER_GLINT:      return "Minecraft Glint";
        case SHADER_MANDELBROT: return "Mandelbrot";
        case SHADER_HOLO:       return "Rainbow Holo";
        case SHADER_GOLD:       return "Golden Foil";
        case SHADER_FIRE:       return "Fire Aura";
        default:                return "Default Crab";
    }
}

Color GetShaderRarityColor(int shader_id) {
    switch (shader_id) {
        case SHADER_GLINT:      return (Color){ 160, 65, 235, 255 };  // Purple (Restricted)
        case SHADER_MANDELBROT: return (Color){ 225, 45, 140, 255 };  // Pink (Classified)
        case SHADER_HOLO:       return (Color){ 50, 195, 245, 255 };  // Cyan Holo
        case SHADER_GOLD:       return (Color){ 245, 195, 35, 255 };  // Gold (Special Rare)
        case SHADER_FIRE:       return (Color){ 235, 50, 40, 255 };   // Red (Covert)
        default:                return (Color){ 130, 140, 160, 255 }; // Gray (Stock)
    }
}

const char* GetShaderRarityName(int shader_id) {
    switch (shader_id) {
        case SHADER_GLINT:      return "RESTRICTED";
        case SHADER_MANDELBROT: return "CLASSIFIED";
        case SHADER_HOLO:       return "HOLOGRAPHIC";
        case SHADER_GOLD:       return "SPECIAL RARE";
        case SHADER_FIRE:       return "COVERT";
        default:                return "STOCK";
    }
}

void InitClickSound(void) {
    if (sound_initialized) return;

    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }

    int sampleRate = 44100;
    int numSamples = 350; // ~8 ms sharp click
    short *data = (short *)malloc(numSamples * sizeof(short));
    if (data == NULL) return;

    for (int i = 0; i < numSamples; i++) {
        float t = (float)i / (float)sampleRate;
        float decay = expf(-t * 900.0f);
        float freq = 2200.0f - t * 60000.0f;
        float val = sinf(2.0f * (float)PI * freq * t) * decay;
        data[i] = (short)(val * 26000.0f);
    }

    Wave wave = {
        .frameCount = (unsigned int)numSamples,
        .sampleRate = (unsigned int)sampleRate,
        .sampleSize = 16,
        .channels = 1,
        .data = data
    };

    click_sound = LoadSoundFromWave(wave);
    free(data);
    sound_initialized = true;
}

void PlayReelClickSound(void) {
    if (sound_initialized && IsAudioDeviceReady()) {
        PlaySound(click_sound);
    }
}
