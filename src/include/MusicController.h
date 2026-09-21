#ifndef MUSIC_CONTROLLER_H
#define MUSIC_CONTROLLER_H

#include "raylib.h"
#include <stdbool.h>

// Initialisiert das Musik-System. (Rufe das 1x am Anfang des Spiels auf)
void InitMusicController(void);

// Updated das Musik-System. (Rufe das JEDEN FRAME in deiner Game-Loop auf)
void UpdateMusicController(void);

// Schliesst das Musik-System. (Rufe das 1x am Ende des Spiels auf)
void CloseMusicController(void);

// Spiele einen Song für eine bestimmte Dauer (in Sekunden).
// Setze durationSeconds auf 0.0f, um das Lied endlos laufen zu lassen.
// Beispiel: PlaySong("assets/Budthansepassportmusik.mp3", 60.0f);
void PlaySong(const char* filepath, float durationSeconds);

// Stoppe die aktuelle Musik
void StopMusic(void);

#endif // MUSIC_CONTROLLER_H
