#pragma once
#include <raylib.h>

inline constexpr const char* paths[1] = {"assets/audio/music/man.mp3"};

inline Music current_music;

inline void playMusic(const int &index) {
    current_music = LoadMusicStream(paths[index]);
    PlayMusicStream(current_music);
}