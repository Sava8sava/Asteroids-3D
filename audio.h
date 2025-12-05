#ifndef AUDIO_H
#define AUDIO_H

void init_audio();
void close_audio();


void play_bgm();
void stop_boost();
void update_boost_sound(bool is_moving);
void play_player_shot();
void play_player_death();
void play_ufo_explosion();
void play_ufo_spawn();
void play_ufo_shot();
void play_asteroid_explosion();


#endif