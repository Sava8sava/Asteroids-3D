#ifndef GAME_H
#define GAME_H 

void init_game_objs();

//global movement variables 
extern bool up,down,left,right,rot_left,rot_right;

void move_player();
void fps_counter();
void calculate_delta();
#endif
