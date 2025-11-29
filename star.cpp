#include "star.h"
#include <cmath>
#include <cstdlib>
#include <stdio.h>


std::vector<Star> starfield;
const int STAR_COUNT = 200; 


static float star_rand(float min, float max) {
    return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

void init_starfield() {
    starfield.clear();
    for(int i = 0; i < STAR_COUNT; i++) {
        Star s;

        s.x = star_rand(-20.0f, 20.0f);
        s.y = star_rand(-15.0f, 15.0f);
        

        s.z = star_rand(-40.0f, -10.0f); 
        

        s.speed = 5.0f / fabs(s.z); 
        
        s.alpha = star_rand(0.5f, 1.0f);
        s.alphaSpeed = star_rand(0.2f, 0.8f);
        
        starfield.push_back(s);
    }
}

void update_starfield(float delta, float pVx, float pVy) {
    // const float BOUND_X = 15.0f;
    // const float BOUND_Y = 11.0f;

    for(auto &s : starfield) {

        s.x -= pVx * s.speed * delta * 0.5f; 
        s.y -= pVy * s.speed * delta * 0.5f;


        s.alpha += s.alphaSpeed * delta;
        if (s.alpha > 1.0f || s.alpha < 0.2f) {
            s.alphaSpeed *= -1;

            if (s.alpha > 1.0f){
                s.alpha = 1.0f;
                s.alphaSpeed *= -1;
            }
            if (s.alpha < 0.3f){
                s.alpha = 0.2f;
                s.alphaSpeed *= -1;
            }
        }


        // if (s.x > BOUND_X) s.x = -BOUND_X;
        // else if (s.x < -BOUND_X) s.x = BOUND_X;
        
        // if (s.y > BOUND_Y) s.y = -BOUND_Y;
        // else if (s.y < -BOUND_Y) s.y = BOUND_Y;
    }
}

void draw_starfield() {
    glPushAttrib(GL_ALL_ATTRIB_BITS); 
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for(const auto &s : starfield) {
        glColor4f(1.0f, 1.0f, 1.0f, s.alpha); 
        glVertex3f(s.x, s.y, s.z);
    }
    glEnd();

    glPopAttrib(); 
}