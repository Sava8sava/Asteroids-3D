#ifndef GAME_H
#define GAME_H 

#include "types.h"

//menu
void draw_text(float x, float y, const char *string, float r,float g, float b);
void draw_menu();
void draw_gameover();
void draw_gui();
void reset_game();
extern Gamestates current_state;
extern bool enter_key_pressed;

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
