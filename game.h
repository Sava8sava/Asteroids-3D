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
void check_P_bullet_ufo_collisions(Ufo *u,int &Points);
void check_U_bullet_player_collisions(Ufo *u,Player *p);
//ações do player 
//global player action variables 

void draw_background();
#endif
