#ifndef GAME_H
#define GAME_H 

#include "types.h"

//gameloop
void init_game_objs();
void fps_counter();
void calculate_delta();

//ações do meteoro 
void init_desenhoMeteoro();  
void check_bullet_meteor_collisions();
void check_bullet_ufo_collisions();
//ações do player 
//global player action variables 
#endif
