#ifndef GAME_H
#define GAME_H 

void init_game_objs();

//global movement variables 
extern bool up,rot_left,rot_right;

void move_player();
void fps_counter();
void calculate_delta();

//adicionei pra facilitar minha vida
void init_desenhoMeteoro();

void draw_spaceship(float size);
#endif
