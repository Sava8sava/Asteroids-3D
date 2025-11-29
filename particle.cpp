#include "meteor.h"
#include "particle.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>

void spawn_explosion(float x, float y, float z) {
    int particleCount = 20;

    for (int i = 0; i < particleCount; i++) {
        Particle p;
        p.x = x; p.y = y; p.z = z;

        p.vx = randRange(-5.0f, 5.0f);
        p.vy = randRange(-5.0f, 5.0f);
        p.vz = randRange(-5.0f, 5.0f);
        
        p.life = 1.0f;
        p.r = 1.0f; 
        p.g = randRange(0.5f, 1.0f); 
        p.b = 0.0f; 
        
        particles.push_back(p);
    }
}

void update_particles(float delta) {
    for (size_t i = 0; i < particles.size(); ) {
        Particle &p = particles[i];
        
        p.x += p.vx * delta;
        p.y += p.vy * delta;
        p.z += p.vz * delta;
        
        p.life -= delta * 2.0f;

        if (p.life <= 0.0f) {
            particles[i] = particles.back();
            particles.pop_back();
        } else {
            i++;
        }
    }
}

void draw_particles() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto &p : particles) {
        glColor3f(p.r, p.g * p.life, p.b * p.life);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopAttrib();
}