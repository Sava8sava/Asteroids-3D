// meteor.cpp
#include "meteor.h"
#include "texture.h"
#include "model.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h>

Model asteroidModel;
bool isAstLoaded;

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
    m->rotSpeed = randRange(-100.0f, 100.0f);
    m->size = randRange(0.4f, 0.9f);
    m->active = true;
}

void initMeteors(std::vector<Meteor>* meteors, int count) {
    if (!isAstLoaded) {
        if (asteroidModel.load("models/10464_Asteroid_L3.123c72035d71-abea-4a34-9131-5e9eeeffadcb/10464_Asteroid_v1_Iterations-2.obj")) {
            isAstLoaded = true;
            asteroidModel.overrideTexture(meteorTexture); 
            printf("Modelo do asteroide carregado com sucesso!\n");
        } else {
            printf("Falha ao carregar modelo do asteroide.\n");
        }
    }
    meteors->clear();
    meteors->reserve(count);
    for (int i = 0; i < count; i++) {
        Meteor m;
        respawnMeteor(&m);
        meteors->push_back(m);
    }
}

void updateMeteors(std::vector<Meteor>* meteors, float deltaTime) {
    const float WRAP_X = BOUNDARY_X + 1.0f;
    const float WRAP_Y = BOUNDARY_Y + 1.0f;

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
        if (m.y < -WRAP_Y) m.y = WRAP_Y;
    }
}

void drawMeteors(const std::vector<Meteor>* meteors) {
    // GLUquadric* quadric = gluNewQuadric();
    // gluQuadricTexture(quadric, GL_TRUE);
    // gluQuadricNormals(quadric, GLU_SMOOTH);

    // glBindTexture(GL_TEXTURE_2D, meteorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
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
            glRotatef(m.rotation, 1.0f, 1.0f, 1.0f);
            // gluSphere(quadric, m.size, 16, 16);
            float vSize = m.size;
            glScalef(vSize, vSize, vSize);
            if(isAstLoaded){
                asteroidModel.draw();
            }
            else{
                glutSolidSphere(0.5, 10, 10);
            }
        glPopMatrix();
    }

    // gluDeleteQuadric(quadric);
}

void splitMeteor(std::vector<Meteor>* meteors, Meteor parent) {
    if (parent.size <= 0.6f) {
        return;
    }

    // cria 3 pedaços
    for (int k = 0; k < 3; k++) {
        Meteor smallM;
        smallM.active = true;

        smallM.x = parent.x;
        smallM.y = parent.y;
        smallM.z = parent.z;

        smallM.size = parent.size * 0.5f;

        float spread = 10.0f;
        smallM.vx = parent.vx + randRange(-spread, spread);
        smallM.vy = parent.vy + randRange(-spread, spread);
        smallM.vz = parent.vz + randRange(8.0f, 12.0f);

        smallM.rotation = randRange(0.0f, 360.0f);
        smallM.rotSpeed = randRange(-50.0f, 50.0f);

        meteors->push_back(smallM);
    }
}
