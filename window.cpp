#include "window.h"
#include "game.h"
#include "player.h"
#include "texture.h"
#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#define ENTER 13

void init_opengl(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    // Configurações de iluminação global
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambient_light[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);

    playerTexture = loadTexture("Texturas/nave.jpg");
    meteorTexture = loadTexture("Texturas/meteoro.jpg");
    propulsorTexture = loadTexture("Texturas/propulsao_nave.jpg");
    backTexture = loadTexture("Texturas/netespaco2.jpg");
    ufoTexture = loadTexture("Texturas/textura_ufo.jpg");

}

void init_window(int argc, char** argv){
    glutInit(&argc,argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Asteroid 3D");

    init_opengl();
    init_desenhoMeteoro();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape_win);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_release);
    glutSpecialFunc(key_press);
    glutSpecialUpFunc(key_release);
    glutIdleFunc(update_game);

}
void keyboard(unsigned char key, int x, int y){
    switch(key){
    case ESC: exit(EXIT_SUCCESS); break;
    case ENTER: enter_key_pressed = true; break;
    case SPACEBAR: spacekey = true; break;
    }
    glutPostRedisplay();
}

void keyboard_release(unsigned char key, int x, int y){
    switch(key){
    case ENTER :
      enter_key_pressed = false; break;
    case SPACEBAR: spacekey = false; break;
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
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //projecão perspectiva 
    gluPerspective(60, (float)w/(float)h, 1.0, 100.0f);
    
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

      case GLUT_KEY_LEFT:
            rot_left = true;
            break;
        
      case GLUT_KEY_RIGHT:
            rot_right = true;
            break;
      }
  }

void key_release(int key, int x, int y){
    switch(key){
      case GLUT_KEY_UP:
            up = false;
            break;
        
      case GLUT_KEY_LEFT:
            rot_left = false;
            break;
        
      case GLUT_KEY_RIGHT:
            rot_right = false;
            break;
      
    }
}

