// meteor.h
#ifndef METEOR_H
#define METEOR_H

#include <GL/glut.h>
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

typedef struct {
    float x, y, z;        // posição
    float vx, vy, vz;     // velocidade
    float rotation;       // ângulo de rotação
    float rotSpeed;       // velocidade de rotação
    float size;           // tamanho do meteoro
    bool active = false;          // se está ativo (para respawn)
} Meteor;

// chama respawnMeteor recursivamente
void initMeteors(std::vector<Meteor>* meteors, int count);

// atualiza meteoros a cada frame
void updateMeteors(std::vector<Meteor>* meteors, float deltaTime);

// desenha todos os meteoros
void drawMeteors(const std::vector<Meteor>* meteors);

// inicializa cada meteoro com as especificações de tamanho e alcance
void respawnMeteor(Meteor* m);

void splitMeteor(std::vector<Meteor>* meteors, Meteor parent); //adicionei pra quantidade de meteoros n ficar pequena

float randRange(float min, float max);
#endif