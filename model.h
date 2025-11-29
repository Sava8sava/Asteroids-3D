#ifndef MODEL_H
#define MODEL_H

#include "glm.h"

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

class Model {
public:
    Model();
    ~Model();

    bool load(const char* filename);

    void draw();

    void setScale(float scale);

    void overrideTexture(GLuint textureID);
private:
    GLMmodel* pModel;
    float currentScale;
};

#endif