#ifndef PLAYER_H
#define PLAYER_H
//Global variables 
#include <cstdint>
#include <vector>
#include "types.h"

extern bool up,rot_left,rot_right,spacekey;

void init_player_var(Player *p);
void draw_player(Player *p);
void move_player(Player *p,float delta);
void reset_player(Player *p);
void draw_spaceship(float size);
void player_shot(std::vector<Bullet> &proj,Player *p);
void update_bullets(std::vector<Bullet> &proj,float delta);
void draw_bullet(std::vector<Bullet> &proj);
bool is_player_alive(Player *p);
#endif
