#ifndef WINDOW_H
#define WINDOW_H

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#define ESC 27

//Declarações das funções da janela 

void init_opengl(void);
void init_window(int argc, char** argv);
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape_win(int w, int h);

#endif
