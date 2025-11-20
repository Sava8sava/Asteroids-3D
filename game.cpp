#include "game.h"
#include "types.h"
#include "texture.h"
#include "meteor.h"
#include "player.h"
#include "Ufo.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector> 
#include <cstdlib>

Player player;
//Lista global de meteoros
std::vector<Meteor> meteors;
//lista global de projetios carregados na tela
std::vector<Bullet> projectiles;
//no jogo original apenas um ufo aparecia na tela por vez
//analisamos mudar isso 
Ufo Zorg;
std::vector<Bullet> ufo_projectiles;

//Fps variables 
static int frame_count = 0;
static float current_time = 0.0f;
static float prev_delta = 0.0f;
static float delta = 0.0f;
static float previous_time = 0.0f;
static float fps = 0.0f;
static int points = 0;

static float ufo_time = 0.0f;

void init_game_objs(){
    //player properties
    init_player_var(&player);  
    //Fps 
    previous_time = glutGet(GLUT_ELAPSED_TIME);
}

void check_collisions_Player_meteor(Player *p) {
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
int cont = 0;
//pequeno teste que move a nave pela janela 
void update_game(void){
    if(is_player_alive(&player)){
      cont++;
      if (cont % 9000 == 0) {
          // progressão de dificuldade papai
          int new_count = 1 + (points / 5000);
          for(int i = 0; i < new_count; i++){
              Meteor m;
              respawnMeteor(&m); // Cria nas bordas
              meteors.push_back(m); // Adiciona à lista existente
          }
      }

      calculate_delta();
      move_player(&player,delta);
      updateMeteors(&meteors, delta);
      

      ufo_time += delta;
      spawn_ufo(&Zorg,ufo_time,points);
      if(Zorg.active){
        //printf("zorg ativo");
        update_ufo(&Zorg,delta);
        Zorg.shoot_timer += delta;
        if(Zorg.shoot_timer >= Zorg.shoot_interval){
          ufo_shot(ufo_projectiles,&Zorg,&player);
        }
      }
       
      if(spacekey){
      player_shot(projectiles,&player);
      spacekey = false;}   
      update_bullets(projectiles,delta);
      update_ufo_bullets(ufo_projectiles,delta);
      check_bullet_meteor_collisions();
      check_collisions_Player_meteor(&player);
      
      fps_counter();
    }else{
      printf("It's over, sobra nada para nave betinha");
      glutLeaveMainLoop();
    }
  //solicita ao glut que a tela seja redesenhada 
  glutPostRedisplay();
}

void draw_background() {
    glPushMatrix();
        glDepthMask(GL_FALSE);

        glBindTexture(GL_TEXTURE_2D, backTexture);
        glColor3f(1.0f, 1.0f, 1.0f);

        glTranslatef(0.0f, 0.0f, -50.0f);

        float bgSizeX = 120.0f;
        float bgSizeY = 90.0f;

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-bgSizeX, -bgSizeY, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( bgSizeX, -bgSizeY, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( bgSizeX,  bgSizeY, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-bgSizeX,  bgSizeY, 0.0f);
        glEnd();

        glDepthMask(GL_TRUE);
    glPopMatrix();
}

void draw_game(void){
  draw_background();
  drawMeteors(&meteors);
  draw_player(&player);
  draw_ufo(&Zorg);
  draw_bullet(projectiles);
  draw_ufo_bullet(ufo_projectiles);
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
      printf("Pontos: %i\n", points);
      previous_time = current_time;
      frame_count = 0;
    }
    
}

void calculate_delta(){
  float curr_delta = glutGet(GLUT_ELAPSED_TIME);
  delta = (curr_delta - prev_delta)/1000.0f; 
  prev_delta = curr_delta;
}

void check_bullet_meteor_collisions() {
  for (size_t i = 0; i < projectiles.size(); ) {
    Bullet &bullet = projectiles[i];
    bool bullet_hit = false;
        
      
      for (size_t j = 0; j < meteors.size(); ++j) {
        Meteor &m = meteors[j];
        if (!m.active) continue; // Pula meteoros inativos
          float bulletRadius = 0.1f; 
          float meteorRadius = m.size;
          //TODO transformar as cordenadas numa struct separada
          // e transforma distSq e sumRadii em funções   
          float deltaX = bullet.x - m.x;
          float deltaY = bullet.y - m.y;
         
          float distSq = (deltaX * deltaX) + (deltaY * deltaY); 

          float sumRadii = bulletRadius + meteorRadius;
          float sumRadiiSq = sumRadii * sumRadii;

          if (distSq < sumRadiiSq) {
              bullet_hit = true;
              points += 100;

              Meteor hitMeteor = m;

              if (j < meteors.size() - 1){
                meteors[j] = meteors.back();
              }
              meteors.pop_back();

              splitMeteor(&meteors, hitMeteor);
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



