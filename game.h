#ifndef GAME_H
#define GAME_H 

void init_game_objs();

//global movement variables 
extern bool up,down,left,right;

void move_player();
void fps_counter();

#endif
