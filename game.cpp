#include "game.h"
#include "window.h"
#include "meteor.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath>
#include <ios>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#define ANGULAR_VEL 180.0f  
#define PI 3.1415926
#define BULLET_SPEED 5.0f
#define MAX_PROJECTILES 5
#define MAX_PRJCT_DISTANCE 10.0f

Entity player;
//Lista global de meteoros
std::vector<Meteor> meteors;
std::vector<Bullet> projectiles;

//Movement variables
bool up = 0;
bool rot_left = 0;
bool rot_right = 0;
bool spacekey = 0;

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

void reset_player(){
    player.x = 0.0f; 
    player.y = 0.0f; 
    player.rotation = 0.0f; 
    player.vx = 0.0f;
    player.vy = 0.0f;
    
    // adicionar lógica de perder vida
}

void check_collisions() {
    float playerRadius = player.size *0.8; 

    for (size_t i = 0; i < meteors.size(); ++i) {
        Meteor &m = meteors[i];
        if (!m.active) continue;
        float meteorRadius = m.size;

        float deltaX = player.x - m.x;
        float deltaY = player.y - m.y;
        float deltaZ = player.z - m.z;

        // (dist_A_B)^2 = (deltaX * deltaX) + (deltaY * deltaY)
        float distSq = (deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ);

        float sumRadii = playerRadius + meteorRadius;
        float sumRadiiSq = sumRadii * sumRadii;

        if (distSq < sumRadiiSq) {
            // barruou
            reset_player();
            respawnMeteor(&m);
        }
    }
}

//pequeno teste que move a nave pela janela 
void update_game(void){
    
    calculate_delta();
    move_player();
    updateMeteors(&meteors, delta);

    if(spacekey){
    player_shot(&player);
    spacekey = false;
    }

    update_bullets();
    check_bullet_collisions();
    fps_counter();
    check_collisions();

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
      draw_spaceship(player.size);
  glPopMatrix();
  
  draw_bullet();
}

void move_player(){
    float accel_amount = player.accel * delta;
    float rotation_rad = player.rotation *(PI/180.0f);

    float frontal_dirx = -sinf(rotation_rad);
    float frontal_diry = cosf(rotation_rad);

    if (up) {
      player.vx += frontal_dirx * accel_amount;
      player.vy += frontal_diry * accel_amount;
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

void init_desenhoMeteoro(){
  initMeteors(&meteors, 10);
  return;
}

void draw_spaceship(float size) {
    float height = size * 2.0f;
    float base_half = size / 2.0f;

    float P[3] = {0.0f, 0.0f, height/2};     // Ponta
    float A[3] = { base_half,  base_half, -height/2};
    float B[3] = {-base_half,  base_half, -height/2};
    float C[3] = {-base_half, -base_half, -height/2};
    float D[3] = { base_half, -base_half, -height/2};
    
    glColor3f(1.0, 0.0, 1.0);

    //BASE
    glBegin(GL_LINE_LOOP);
        glVertex3fv(A);
        glVertex3fv(B);
        glVertex3fv(C);
        glVertex3fv(D);
    glEnd();

    // LATERAIS 
    // Desenhar as arestas laterais
    glBegin(GL_LINES);
        glVertex3fv(P); glVertex3fv(A);
        glVertex3fv(P); glVertex3fv(B);
        glVertex3fv(P); glVertex3fv(C);
        glVertex3fv(P); glVertex3fv(D);
    glEnd();
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

void player_shot(Entity *p){

  //checa se ja tem 5 tiros na tela
  if(projectiles.size() >= MAX_PROJECTILES){return ;}

  //calcula a direção que o player apontando
  float player_angle = (p->rotation)  * (PI/ANGULAR_VEL);
  float shot_dirx = -sinf(player_angle);
  float shot_diry = cos(player_angle); 
  Bullet new_bullet;

  new_bullet.x = p->x;
  new_bullet.y = p->y;
  new_bullet.z = p->z;
  new_bullet.Vx = shot_dirx * BULLET_SPEED;
  new_bullet.Vy = shot_diry * BULLET_SPEED;

  projectiles.push_back(new_bullet);
}

void update_bullets(){
  for(size_t i = 0; i < projectiles.size(); ){
    Bullet &b = projectiles[i];

    b.x += b.Vx * delta;
    b.y += b.Vy * delta;
    b.z += b.Vz * delta;
  
  if(fabs(b.x) > MAX_PRJCT_DISTANCE || fabs(b.y) > MAX_PRJCT_DISTANCE ){
     projectiles.erase(projectiles.begin() + i); 
  }else{
      ++i;
    } 
  }
}

void draw_bullet(){
  glColor3f(1.0f,0.0f,0.0f);
  for (const auto& bullet : projectiles){
    glPushMatrix();
      glTranslated(bullet.x,bullet.y,bullet.z);
      glRotatef(90.0,-1.0,0.0,0.0);
      glPointSize(4.0f);
      glBegin(GL_POINTS);
      glVertex3f(0.0f,0.0f,0.0f);
      glEnd();
    glPopMatrix();
  }

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
