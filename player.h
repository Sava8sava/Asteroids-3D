#ifndef PLAYER_H
#define PLAYER_H
//Global variables 
#include <cstdint>
#include <vector>
#include "types.h"

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/freeglut.h>
    #include <GL/freeglut_std.h>
#endif

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
