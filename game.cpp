#include "game.h"
#include "window.h"
#include "meteor.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <ios>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#define ANGULAR_VEL 180.0f  

//Global variables 
typedef struct{
    float x,y,z;
    float rotation;
    float size;
    float vx,vy;
    float accel; 
    float damping_rate;
} Entity; 

Entity player;
//Lista global de meteoros
std::vector<Meteor> meteors;

//Movement variables
bool up = 0;
bool down = 0;
bool left = 0;
bool right = 0;
bool rot_left = 0;
bool rot_right = 0;

//Fps variables 
static int frame_count = 0;
static float current_time = 0.0f;
static float prev_delta = 0.0f;
static float delta = 0.0f;
static float previous_time = 0.0f;
static float fps = 0.0f;

void init_game_objs(){
    //player properties
    player.x = 0.0f; 
    player.y = 0.0f; 
    player.z = 0.0f;
    player.rotation = 0.0f; 
    player.size = 0.3f; 
    player.vx = 0.0f;
    player.vy = 0.0f;
    player.accel = 10.0f; 
    player.damping_rate = 0.95f;
    
    //Fps 
    previous_time = glutGet(GLUT_ELAPSED_TIME);
}

//pequeno teste que move a nave pela janela 
void update_game(void){
    
    calculate_delta();
    updateMeteors(&meteors, delta);
    move_player();
    fps_counter(); 

  //solicita ao glut que a tela seja redesenhada 
  glutPostRedisplay();
}

void draw_game(void){
  drawMeteors(&meteors);
  glPushMatrix();
      glTranslated(player.x,player.y,player.z);
      glRotatef(player.rotation,0.0,0.0,1.0);
      glRotatef(90.0,-1.0,0.0,0.0);
      glColor3f(1.0,0.0,1.0);
      //glutWireCube(player.size);
      glutWireCone(player.size,player.size * 2,10,10);
  glPopMatrix();

}
//o asteroids original tem movimentação de tanque 
//por enquanto manteremos movimentação omnidirecional
void move_player(){
    float accel_amount = player.accel * delta;
     
     
    if(left && right) {
    //do nothing
    }
    if(up && down) {
    //do nothing
    }
    if (up) {
      player.vy += accel_amount;
    }
    if (down) {
      player.vy -= accel_amount;
    }
    if (left) {
      player.vx -= accel_amount;
    }
    if (right) {
      player.vx += accel_amount;
    }
    if (rot_left){
      player.rotation += ANGULAR_VEL * delta;
    }
    if(rot_right){
      player.rotation -= ANGULAR_VEL * delta;
    }
    
    player.vx *= player.damping_rate;
    player.vy *= player.damping_rate;
    
    player.x += player.vx * delta;
    player.y += player.vy * delta;

    if (fabs(player.vx) < 0.001f) player.vx = 0.0f;
    if (fabs(player.vy) < 0.001f) player.vy = 0.0f;
    if (player.rotation > 360.0f) player.rotation -= 360.0f;
    if (player.rotation < 0.0f) player.rotation += 360.0f;

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

void calculate_delta(){
    float curr_delta = glutGet(GLUT_ELAPSED_TIME);
    delta = (curr_delta - prev_delta)/1000.0f; 
    prev_delta = curr_delta;
} 

void init_desenhoMeteoro(){
  initMeteors(&meteors, 20);
  return;
}