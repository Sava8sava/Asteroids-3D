#include "audio.h"
#include <stdio.h>

#define MINIAUDIO_IMPLEMENTATION 
#include "miniaudio.h"

// Motor de áudio global
ma_engine engine;

ma_sound boost_sound; 

bool audio_initialized = false;
bool boost_playing = false;

void init_audio() {
    ma_result result;

    // Inicializa o motor de áudio
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Falha ao inicializar audio.\n");
        return;
    }

    // Carrega o som do boost na memória mas deixa pausado/em loop
    result = ma_sound_init_from_file(&engine, "audio/boost.mp3", 0, NULL, NULL, &boost_sound);
    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&boost_sound, MA_TRUE);
        ma_sound_set_volume(&boost_sound, 0.5f);
    }

    audio_initialized = true;
}

void close_audio() {
    if (!audio_initialized) return;
    ma_sound_uninit(&boost_sound);
    ma_engine_uninit(&engine);
}

void play_bgm() {
    if (!audio_initialized) return;
    static ma_sound music;
    ma_sound_init_from_file(&engine, "audio/background.mp3", 0, NULL, NULL, &music);
    ma_sound_set_looping(&music, MA_TRUE);
    ma_sound_set_volume(&music, 0.35f);
    ma_sound_start(&music);
    }

void update_boost_sound(bool is_moving) {
    if (!audio_initialized) return;

    if (is_moving) {
        if (!ma_sound_is_playing(&boost_sound)) {
            ma_sound_start(&boost_sound);
        }
    } else {
        if (ma_sound_is_playing(&boost_sound)) {
            ma_sound_stop(&boost_sound);
        }
    }
}

void play_player_shot() {
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/tiro.wav", NULL);
}

void play_player_death() {
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/ship_death.mp3", NULL);
}

void play_ufo_explosion() {
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/ufo_explosion.mp3", NULL);
}

void play_ufo_spawn() {
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/spawn_ufo.mp3", NULL);
}

void play_ufo_shot() {
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/ufo_shot.wav", NULL);
}

void play_asteroid_explosion(){
    if (!audio_initialized) return;
    ma_engine_play_sound(&engine, "audio/meteor_explosion.mp3", NULL);
}