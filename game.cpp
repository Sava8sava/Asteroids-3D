#include "game.h"
#include "window.h"
#include "meteor.h"
#include "player.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>



Player player;
//Lista global de meteoros
std::vector<Meteor> meteors;
//lista global de projetios carregados na tela
std::vector<Bullet> projectiles;

//Fps variables 
static int frame_count = 0;
static float current_time = 0.0f;
static float prev_delta = 0.0f;
static float delta = 0.0f;
static float previous_time = 0.0f;
static float fps = 0.0f;

void init_game_objs(){
    //player properties
    init_player_var(&player);  
    //Fps 
    previous_time = glutGet(GLUT_ELAPSED_TIME);
}

void check_collisions(Player *p) {
    float playerRadius = p->size *0.8; 

    for (size_t i = 0; i < meteors.size(); ++i) {
        Meteor &m = meteors[i];
        if (!m.active) continue;
        float meteorRadius = m.size;

        float deltaX = p->x - m.x;
        float deltaY = p->y - m.y;
        float deltaZ = p->z - m.z;

        // (dist_A_B)^2 = (deltaX * deltaX) + (deltaY * deltaY)
        float distSq = (deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ);

        float sumRadii = playerRadius + meteorRadius;
        float sumRadiiSq = sumRadii * sumRadii;

        if (distSq < sumRadiiSq) {
            // barruou
            reset_player(&player);
            respawnMeteor(&m);
        }
    }
}

//pequeno teste que move a nave pela janela 
void update_game(void){
    if(is_player_alive(&player)){
      calculate_delta();
      move_player(&player,delta);
      updateMeteors(&meteors, delta);

      if(spacekey){
      player_shot(projectiles,&player);
      spacekey = false;
      }

      update_bullets(projectiles,delta);
      check_bullet_collisions();
      fps_counter();
      check_collisions(&player);
    }else{
      printf("It's over, sobra nada para nave betinha");
      glutLeaveMainLoop();
    }
  //solicita ao glut que a tela seja redesenhada 
  glutPostRedisplay();
}

void draw_game(void){
  drawMeteors(&meteors);
  draw_player(&player);  
  draw_bullet(projectiles);
}

void init_desenhoMeteoro(){
  initMeteors(&meteors, 10);
  return;
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

void check_bullet_collisions() {
  for (size_t i = 0; i < projectiles.size(); ) {
    Bullet &bullet = projectiles[i];
    bool bullet_hit = false;
        
      
      for (size_t j = 0; j < meteors.size(); ++j) {
        Meteor &m = meteors[j];
        if (!m.active) continue; // Pula meteoros inativos
          float bulletRadius = 0.1f; 
          float meteorRadius = m.size;

          float deltaX = bullet.x - m.x;
          float deltaY = bullet.y - m.y;
         
          float distSq = (deltaX * deltaX) + (deltaY * deltaY); 

          float sumRadii = bulletRadius + meteorRadius;
          float sumRadiiSq = sumRadii * sumRadii;

          if (distSq < sumRadiiSq) {
            respawnMeteor(&m);                 
              bullet_hit = true;
              break; // A bala acertou, pode parar de checar contra outros meteoros
          }
      }
        
        if (bullet_hit) {
            projectiles.erase(projectiles.begin() + i);
        } else {
            // o tiro não pegou em niguem
            ++i; 
        }
    }
}
