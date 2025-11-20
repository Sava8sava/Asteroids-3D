#ifndef TEXTURE_H
#define TEXTURE_H
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
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

extern GLuint playerTexture;
extern GLuint meteorTexture;
extern GLuint propulsorTexture;

GLuint loadTexture(const char* filename);

#endif