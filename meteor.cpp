// meteor.cpp
#include "meteor.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h>

float randRange(float min, float max) {
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

/*
spawn ocorre atrás do jogador, longe da câmera e os meteoros
vão se aproximando da nave. Ao sair da tela, os asteroides respawnam
até serem destruídos
*/

const float BOUNDARY_X = 8.0f;
const float BOUNDARY_Y = 6.0f;

void respawnMeteor(Meteor* m){
    
    if (rand() % 2 == 0) {
        m->x = (rand() % 2 == 0) ? BOUNDARY_X + 2.0f : -BOUNDARY_X - 2.0f;
        m->y = randRange(-BOUNDARY_Y, BOUNDARY_Y);
    } else {
        m->y = (rand() % 2 == 0) ? BOUNDARY_Y + 2.0f : -BOUNDARY_Y - 2.0f;
        m->x = randRange(-BOUNDARY_X, BOUNDARY_X);
    }
    m->z = 0.0f;
    float speed = randRange(1.0f, 4.0f);
    m->vx = (m->x > 0) ? randRange(-speed, -0.5f) : randRange(0.5f, speed);
    m->vy = (m->y > 0) ? randRange(-speed, -0.5f) : randRange(0.5f, speed);
    m->vz = 0.0f;
    m->rotation = randRange(0.0f, 360.0f);
    m->rotSpeed = randRange(-4.0f, 4.0f);
    m->size = randRange(0.4f, 0.9f);
    m->active = true;
}

void initMeteors(std::vector<Meteor>* meteors, int count) {
    meteors->clear();
    meteors->reserve(count);
    for (int i = 0; i < count; i++) {
        Meteor m;
        respawnMeteor(&m);
        meteors->push_back(m);
    }
}

void updateMeteors(std::vector<Meteor>* meteors, float deltaTime) {
    const float WRAP_X = BOUNDARY_X + 4.0f;
    const float WRAP_Y = BOUNDARY_Y + 4.0f;

    for (size_t i = 0; i < meteors->size(); ++i) {
        Meteor &m = (*meteors)[i];
        if (!m.active) continue;

        m.x += m.vx * deltaTime;
        m.y += m.vy * deltaTime;

        m.rotation += m.rotSpeed * deltaTime;

        // if (i == 0) {
        //     printf("[UPDATE] id=%zu Pos(%.2f, %.2f, %.2f) Vel(%.2f, %.2f, %.2f)\n",
        //            i, m.x, m.y, m.z, m.vx, m.vy, m.vz);
        //     fflush(stdout);
        // }
        
        if (m.x > WRAP_X) m.x = -WRAP_X;
        if (m.x < -WRAP_X) m.x = WRAP_X;
        if (m.y > WRAP_Y) m.y = -WRAP_Y;
        if (m.y < -WRAP_Y) m.y = -WRAP_Y;
    }
}

void drawMeteors(const std::vector<Meteor>* meteors) {
    glColor3f(0.5f,0.5f,0.5f);
    for (size_t i = 0; i < meteors->size(); ++i) {
        const Meteor &m = (*meteors)[i];
        if (!m.active) continue;

        // Log quando estiver na zona próxima — útil pra confirmar visibilidade
        // if (i == 0 && m.z > -10.0f && m.z < 10.0f) {
        //     printf("[POTENTIALLY_VISIBLE] id=%zu Pos(%.2f, %.2f, %.2f) Size=%.2f\n",
        //            i, m.x, m.y, m.z, m.size);
        //     fflush(stdout);
        // }

        glPushMatrix();
            glTranslatef(m.x, m.y, m.z);
            glRotatef(m.rotation, 0.0f, 1.0f, 0.0f);
            glutSolidSphere(m.size, 16, 16);
        glPopMatrix();
    }
}

