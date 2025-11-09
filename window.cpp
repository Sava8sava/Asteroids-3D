#include "window.h"
#include "game.h"
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
    glutSpecialFunc(key_press);
    glutSpecialUpFunc(key_release);
    glutIdleFunc(update_game);

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
    draw_game();
    
    //glColor3f(1.0,0.0,0.0);
    //glutWireCube(1.0);
    
    glutSwapBuffers();
}

void reshape_win(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    //projecão perspectiva 
    gluPerspective(60, (float)w/(float)h, 1.0, 10.0f);
    
        gluLookAt(0.0,0.0,9.0,//posição do olho
              0.0,0.0,0.0, //direção da camera 
              0.0,1.0,0.0); // sentido da camera 
    glMatrixMode(GL_MODELVIEW);
}

void key_press(int key, int x, int y){
    switch(key){
      case GLUT_KEY_UP:
            up = true;
            break;
        
      case GLUT_KEY_DOWN:
            down = true;
            break;
        
      case GLUT_KEY_LEFT:
            rot_left = true;
            break;
        
      case GLUT_KEY_RIGHT:
            rot_right = true;
            break;
      
      case 'q':
      case 'Q':
            rot_left = true;
            break;

      case 'w':
      case 'W':
            rot_right = true; 
            break;
      }
}
//TODO: A rotação não esta funcionando pq a função esta usando teclas normais e não especiais 
// no caso vou mudar isso quando adicionar a movimentação de tanque 
void key_release(int key, int x, int y){
    switch(key){
      case GLUT_KEY_UP:
            up = false;
            break;
        
      case GLUT_KEY_DOWN:
            down = false;
            break;
        
      case GLUT_KEY_LEFT:
            rot_left = false;
            break;
        
      case GLUT_KEY_RIGHT:
            rot_right = false;
            break;
      
      case 'q':
      case 'Q':
            rot_left = false;
            break;

      case 'w':
      case 'W':
            rot_right = false; 
            break;
      }
}
