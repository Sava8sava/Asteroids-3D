// meteor.cpp
#include "meteor.h"
#include <cmath>
#include <cstdlib>

float randRange(float min, float max) {
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

void initMeteors(std::vector<Meteor>* meteors, int count) {
    meteors->clear();
    for (int i = 0; i < count; i++) {
        Meteor m;
        m.x = randRange(-5.0f, 5.0f);
        m.y = randRange(-5.0f, 5.0f);
        m.z = randRange(-5.0f, 5.0f);

        m.vx = randRange(-2.0f, 2.0f);
        m.vy = randRange(-2.0f, 2.0f);
        m.vz = randRange(-2.0f, 2.0f);

        m.rotation = randRange(0.0f, 360.0f);
        m.rotSpeed = randRange(-4.0f, 4.0f);
        m.size = randRange(0.5f, 2.0f);
        m.active = true;
        meteors->push_back(m);
    }
}

void updateMeteors(std::vector<Meteor>* meteors, float deltaTime) {
    for (auto& m : *meteors) {
        if (!m.active) continue;
        m.x += m.vx * deltaTime;
        m.y += m.vy * deltaTime;
        m.z += m.vz * deltaTime;
        m.rotation += m.rotSpeed * deltaTime;

        // Respawn se sair dos limites
        if (fabs(m.x) > 100 || fabs(m.y) > 100 || fabs(m.z) > 100) {
            m.x = randRange(-5.0f, 5.0f);
            m.y = randRange(-5.0f, 5.0f);
            m.z = randRange(-5.0f, 5.0f);
        }
    }
}

void drawMeteors(const std::vector<Meteor>* meteors) {
    for (const auto& m : *meteors) {
        if (!m.active) continue;
        glPushMatrix();
            glTranslatef(m.x, m.y, m.z);
            glRotatef(m.rotation, 0.0f, 1.0f, 0.0f);
            glutSolidSphere(m.size, 12, 12);
        glPopMatrix();
    }
}
