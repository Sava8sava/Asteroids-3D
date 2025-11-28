#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>

struct Particle {
    float x, y, z;      // Posição
    float vx, vy, vz;   // Velocidade
    float life;         // Vida (1.0 = nova, 0.0 = morta)
    float r, g, b;      // Cor
};
extern std::vector<Particle> particles;

void draw_particles();
void update_particles(float delta);
void spawn_explosion(float x, float y, float z);

#endif