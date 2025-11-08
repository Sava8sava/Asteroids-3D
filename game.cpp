#include "game.h"
#include "window.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <ios>
#include <stdlib.h>
#include <stdio.h>


//Global variables 
typedef struct{
    float x,y,z;
    float rotation;
    float size;
    float velocity;
} Entity; 

Entity player;

//Movement variables
 bool up = 0;
 bool down = 0;
 bool left = 0;
 bool right = 0;

//Fps variables 
static int frame_count = 0;
static float current_time = 0.0f;
static float previous_time = 0.0f;
static float fps = 0.0f;

void init_game_objs(){
    //player properties
    player.x = 0.0f; 
    player.y = 0.0f; 
    player.z = 0.0f;
    player.rotation = 0.0f; 
    player.size = 0.3f; 
    player.velocity = 0.05f;

    //Fps 
    previous_time = glutGet(GLUT_ELAPSED_TIME);
}

//pequeno teste que move a nave pela janela 
void update_game(void){
    
    move_player();
    fps_counter(); 

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
//o asteroids original tem movimentação de tanque 
//por enquanto manteremos movimentação omnidirecional
void move_player(){
    if(left && right) {
    //do nothing
    }
    if(up && down) {
    //do nothing
    }
    if (up) {
      player.y += player.velocity;
    }
    if (down) {
      player.y -= player.velocity;
    }
    if (left) {
      player.x -= player.velocity;
    }
    if (right) {
      player.x += player.velocity;
    }
   //TODO: corrigir o movimento diagonal mais rapido, pesquisar como normalizar  
}

void fps_counter(){
    frame_count++;
    current_time = glutGet(GLUT_ELAPSED_TIME);
    
    //milliseconds
    if(current_time - previous_time > 1000.0f){
      fps = frame_count/((current_time - previous_time)/1000.0f);
      printf("FPS: %.2f\n", fps);
      previous_time = current_time;
      frame_count = 0;
    }
    
}
