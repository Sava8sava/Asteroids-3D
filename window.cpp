#include "window.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

void init_opengl(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
}

void init_window(int argc, char** argv){
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Asteroid 3D");

    init_opengl();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_win);
    glutKeyboardFunc(keyboard);

}
void keyboard(unsigned char key, int x, int y){
    switch(key){
    case ESC: exit(EXIT_SUCCESS); break;
    }
    glutPostRedisplay();
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    //LOGICA DO JOGO APLICADO AQUI 
    
    glColor3f(1.0,0.0,0.0);
    glutWireCube(1.0);

    glutSwapBuffers();
}

void reshape_win(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    //projecão perspectiva 
    gluPerspective(60, (float)w/(float)h, 1.0, 9.0);
    
    gluLookAt(0.0,0.0,7.0,//posição do olho
              0.0,1.0,0.0, //direção da camera 
              0.0,1.0,0.0); // sentido da camera 
    glMatrixMode(GL_MODELVIEW);
}

