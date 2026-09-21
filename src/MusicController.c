#include "include/MusicController.h"
#include <stddef.h>

static Music currentMusic = { 0 };
static bool isMusicLoaded = false;
static float playTimer = 0.0f;
static float targetDuration = 0.0f;
static bool isPlayingForDuration = false;

void InitMusicController(void) {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
}

void UpdateMusicController(void) {
    if (isMusicLoaded && IsMusicStreamPlaying(currentMusic)) {
        UpdateMusicStream(currentMusic);
        
        if (isPlayingForDuration) {
            playTimer += GetFrameTime();
            if (playTimer >= targetDuration) {
                StopMusicStream(currentMusic);
                isPlayingForDuration = false;
            }
        }
    }
}

void CloseMusicController(void) {
    if (isMusicLoaded) {
        UnloadMusicStream(currentMusic);
        isMusicLoaded = false;
    }
    
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

void PlaySong(const char* filepath, float durationSeconds) {
    if (!IsAudioDeviceReady()) return;

    // Falls schon ein Song läuft, stoppen und entladen
    if (isMusicLoaded) {
        StopMusicStream(currentMusic);
        UnloadMusicStream(currentMusic);
        isMusicLoaded = false;
    }
    
    currentMusic = LoadMusicStream(filepath);
    isMusicLoaded = true;
    PlayMusicStream(currentMusic);
    
    // Timer setzen falls wir eine spezifische Dauer haben
    if (durationSeconds > 0.0f) {
        isPlayingForDuration = true;
        targetDuration = durationSeconds;
        playTimer = 0.0f;
    } else {
        isPlayingForDuration = false;
    }
}

void StopMusic(void) {
    if (isMusicLoaded) {
        StopMusicStream(currentMusic);
        isPlayingForDuration = false;
    }
}
