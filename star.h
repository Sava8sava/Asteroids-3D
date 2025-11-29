#ifndef STAR_H
#define STAR_H

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
struct Star {
    float x, y, z;      // posição (Z define profundidade/velocidade)
    float speed;        // fator de movimento (Paralaxe)
    float alpha;        // brilho atual
    float alphaSpeed;   // velocidade do piscar
};

void init_starfield();
void update_starfield(float delta, float pVx, float pVy);
void draw_starfield();

#endif