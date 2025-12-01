#ifndef UFO_H
#define UFO_H
#include <cstdint>
#include "types.h"
#include <vector>
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
/**
O jogo original conta com dois tipos de inimigos:
Big_ufos -> grande, lento e burro, atiram aleatoriamente 
Mini_ufos -> pequenos rapidos e esperto, miram no player
**/

Ufos_types ufo_spawn_prob(int points);
void init_ufo(Ufo *u,int points);
void update_ufo(Ufo *u,float delta);
void draw_ufo(Ufo *u);
void spawn_ufo(Ufo *u,float &ufo_time,int points);
void ufo_shot(std::vector<Bullet> &ufo_proj,Ufo *u,Player *p);
void update_ufo_bullets(std::vector<Bullet> &ufo_proj,float delta);
void draw_ufo_bullet(std::vector<Bullet> &ufo_proj);
#endif 
