#include "game.h"
#include "window.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct{
    float x,y,z;
    float rotation;
    float size;
} Entity; 

Entity player;

void init_game_objs(){
    player.x = 0.0f; 
    player.y = 0.0f; 
    player.z = 0.0f;
    player.rotation = 0.0f; 
    player.size = 0.3f; 

}

//pequeno teste que move a nave pela janela 
void update_game(void){
    player.x += 0.001f;
    
    if(player.x > 1.0f){
        player.x = -1.0f;
    }
  //solicita ao glut que a tela seja redesenhada 
  glutPostRedisplay();
}

void draw_game(void){
  
  glPushMatrix();
      glTranslated(player.x,player.y,player.z);
      glRotatef(90.0,1.0,0.0,0.0);
      glColor3f(1.0,0.0,1.0);
      glutWireCone(player.size,player.size * 2,10,10);
  glPopMatrix();

}
