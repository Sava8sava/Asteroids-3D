// meteor.h
#ifndef METEOR_H
#define METEOR_H

#include <GL/glut.h>
#include <vector>

typedef struct {
    float x, y, z;        // posição
    float vx, vy, vz;     // velocidade
    float rotation;       // ângulo de rotação
    float rotSpeed;       // velocidade de rotação
    float size;           // tamanho do meteoro
    bool active;          // se está ativo (para respawn)
} Meteor;

// inicializa meteoros
void initMeteors(std::vector<Meteor>* meteors, int count);

// atualiza meteoros a cada frame
void updateMeteors(std::vector<Meteor>* meteors, float deltaTime);

// desenha todos os meteoros
void drawMeteors(const std::vector<Meteor>* meteors);

#endif