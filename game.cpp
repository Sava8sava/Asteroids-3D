#include "game.h"
#include "types.h"
#include "texture.h"
#include "meteor.h"
#include "player.h"
#include "Ufo.h"
#include "window.h"
#include "particle.h"
#include "star.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector> 
#include <cstdlib>

#define FIXED_TIMESTEP 0.01666f
#define SPAWN_INTERVAL 120.0f 
Gamestates current_state = GAME_OVER;
bool enter_key_pressed = false;

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
static float accum = 0.0f;
static float ufo_time = 0.0f;
static float spawm_timer = 0.0f;

//partículas de explosão
std::vector<Particle> particles;

void init_game_objs(){
    //player properties
    init_player_var(&player);
    init_starfield();
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
            spawn_explosion(p->x, p->y, p->z);
            reset_player(&player);
            respawnMeteor(&m);
        }
    }
}
//refatorar 
void update_game(void){
  switch (current_state) {
    case MENU:
      if(enter_key_pressed){
        reset_game();
        current_state = PLAYING;
        enter_key_pressed = false;
      }
      break;

    case PLAYING:
      if(is_player_alive(&player)){
        calculate_delta();
        accum += delta;
        if (accum > 0.2f){
          accum = 0.2f;
        }

        while(accum >= FIXED_TIMESTEP){
          
          spawm_timer += FIXED_TIMESTEP;
          //progressão de dificuldade em relação os pontos 
          float curr_interval = SPAWN_INTERVAL - (points/500.0f);
          if(curr_interval < 10.0f)curr_interval = 10.0f;

          if (spawm_timer >= curr_interval) {
            spawm_timer = 0.0f;
            int new_count = 1 + (points / 1000);
            for(int i = 0; i < new_count; i++){
                Meteor m;
                respawnMeteor(&m); // Cria nas bordas
                meteors.push_back(m); // Adiciona à lista existente
            }
        }

          move_player(&player,FIXED_TIMESTEP);
          update_starfield(FIXED_TIMESTEP, player.vx, player.vy);
          updateMeteors(&meteors, FIXED_TIMESTEP);

          ufo_time += FIXED_TIMESTEP;
          spawn_ufo(&Zorg,ufo_time,points);

          if(Zorg.active){
            //printf("zorg ativo");
            update_ufo(&Zorg,FIXED_TIMESTEP);
            Zorg.shoot_timer += FIXED_TIMESTEP;
            if(Zorg.shoot_timer >= Zorg.shoot_interval){
              ufo_shot(ufo_projectiles,&Zorg,&player);
              Zorg.shoot_timer = 0.0f;
            }
          }
           
          if(spacekey){
            player_shot(projectiles,&player);
            spacekey = false;
          }

          update_bullets(projectiles,FIXED_TIMESTEP);
          update_ufo_bullets(ufo_projectiles,FIXED_TIMESTEP);
          check_P_bullet_ufo_collisions(&Zorg,points);
          check_U_bullet_player_collisions(&Zorg,&player);
          check_bullet_meteor_collisions();
          check_collisions_Player_meteor(&player);
          accum -= FIXED_TIMESTEP;
          }
          fps_counter();
          update_particles(delta);
      }else{
        printf("It's over, sobra nada para nave betinha");
        //glutLeaveMainLoop();
        current_state = GAME_OVER;

      }
      break ;
    case GAME_OVER:
      if(enter_key_pressed){
        current_state = MENU;
        enter_key_pressed = false;
      }
      break;
  }
  //solicita ao glut que a tela seja redesenhada 
  glutPostRedisplay();
}

void draw_background() {
    glPushMatrix();
        glDepthMask(GL_FALSE);

        glBindTexture(GL_TEXTURE_2D, backTexture);
        glColor3f(0.75f, 0.75f, 0.75f);

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

  switch (current_state) {
    case MENU:
      draw_menu();
      break;
    case PLAYING:
      glDisable(GL_LIGHTING); 
      draw_background();
      draw_starfield();
      glEnable(GL_LIGHTING);
      // glEnable(GL_COLOR_MATERIAL);
      glColor3f(1.0f, 1.0f, 1.0f);


      drawMeteors(&meteors);
      draw_player(&player);
      draw_ufo(&Zorg);
      draw_bullet(projectiles);
      draw_ufo_bullet(ufo_projectiles);
      draw_gui();
      draw_particles();
      break;
    case GAME_OVER:
      draw_gameover();
      break;
    }
  //glutSwapBuffers();
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

              spawn_explosion(m.x, m.y, m.z);

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
            projectiles[i] = projectiles.back();
            projectiles.pop_back();
        } else {
            // o tiro não pegou em niguem
            ++i;
        }
    }
}

void check_P_bullet_ufo_collisions(Ufo *u, int &points) {
  if(!u->active){
    return;
  }
  float ufoRadius = u->size * 0.8f;

  for (size_t i = 0; i < projectiles.size(); ) {
    Bullet &bullet = projectiles[i];      

        float bulletRadius = 0.1f; 
    
        //TODO transformar as cordenadas numa struct separada
        // e transforma distSq e sumRadii em funções
        
        float deltaX = bullet.x - u->x;
        float deltaY = bullet.y - u->y;
       
        float distSq = (deltaX * deltaX) + (deltaY * deltaY); 

        float sumRadii = bulletRadius + ufoRadius;
        float sumRadiiSq = sumRadii * sumRadii;

        if (distSq < sumRadiiSq) {
          if(u->type == BIG_UFO){
            points += 200;
          }else{
            points += 500;
          }
          spawn_explosion(u->x, u->y, u->z);
          u->active = false;
          projectiles[i] = projectiles.back();
          projectiles.pop_back();
            return;
        }else{
          i++;
    }
  }
}

void check_U_bullet_player_collisions(Ufo *u, Player *p) {
  if(!u->active){
    return;
  }

  float playerRadius = p->size * 0.5f;

  for (size_t i = 0; i < ufo_projectiles.size(); ) {
    Bullet &bullet = ufo_projectiles[i];      

        float bulletRadius = 0.1f; 
    
        //TODO transformar as cordenadas numa struct separada
        // e transforma distSq e sumRadii em funções
        
        float deltaX = bullet.x - p->x;
        float deltaY = bullet.y - p->y;
       
        float distSq = (deltaX * deltaX) + (deltaY * deltaY); 

        float sumRadii = bulletRadius + playerRadius;
        float sumRadiiSq = sumRadii * sumRadii;

        if (distSq < sumRadiiSq) {
          spawn_explosion(p->x, p->y, p->z);
          reset_player(p);
          ufo_projectiles[i] = ufo_projectiles.back();
          ufo_projectiles.pop_back();
        }else{
          i++;
    }
  }
}

void reset_game(){
    points = 0;
    ufo_time = 0.0f;
    spawm_timer = 0.0f; 
    // Limpa os vetores de objetos
    meteors.clear();
    projectiles.clear();
    ufo_projectiles.clear();
    particles.clear();
    init_player_var(&player);     
    init_desenhoMeteoro();     
    Zorg.active = false;
}

void draw_text(float x, float y, const char *string, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColor3f(r,g,b);
    glPushMatrix();
        glTranslatef(x, y, -10.0f);
        glScalef(0.01f, 0.01f, 0.01f); 
        for (const char *c = string; *c != '\0'; c++) {
          //igual na idade da pedra lascada   
          glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

//TODO alguma coisa na função de desenhar o exaustor e na função de draw_background que tem dentro de game over e munu esta mudando a cor do texto
void draw_menu() {
    draw_background();
    draw_text(-7.0f, 2.5f, "ASTEROIDS CLONE", 1.0f, 0.0f, 1.0f);
    draw_text(-8.0f, -2.5f, "PRESS ENTER TO START", 1.0f,1.0f,1.0f);
}

void draw_gameover() {
    //draw_background(); 
    draw_text(-4.0f, 2.5f, "GAME OVER", 1.0f, 0.0f, 0.0f);
    char score_text[50];
    sprintf(score_text, "FINAL SCORE: %d", points);
    draw_text(-5.5f, -2.0f, score_text, 0.0f, 1.0f, 0.0f);
    draw_text(-8.5f, -3.5f, "PRESS ENTER TO RESTART", 0.0f, 1.0f, 0.0f);
}

void draw_gui(){
  char score_text[32];
  sprintf(score_text,"Score %d",points);
  draw_text(-13.0f,9.0f,score_text,1.0f,1.0f,1.0f);

  char lives_text[32];
  sprintf(lives_text,"Lives %d",player.lives);
  draw_text(7.0f,9.0f,lives_text,1.0f,1.0f,1.0f);
}
