#include <stdio.h>
#include <math.h>
#include "player.h"
#include "texture.h"
#include "model.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <vector>

#define BULLET_SPEED 20.0f
#define MAX_PROJECTILES 5
#define MAX_PRJCT_DISTANCE 10.0f
#define PLAYER_SIZE 0.5f
#define PLAYER_LIVES 10 //o valor maximo é de um byte 

bool up = 0;
bool rot_left = 0;
bool rot_right = 0;
bool spacekey = 0;

Model playerShip;
bool shipLoaded = false;

void init_player_var(Player *p){
    p->x = 0.0f; 
    p->y = 0.0f; 
    p->z= 0.0f;
    p->rotation = 0.0f; 
    p->size = PLAYER_SIZE; 
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->accel = 85.0f; 
    p->damping_rate = 0.95f;
    p->lives = PLAYER_LIVES;

    if (!shipLoaded) {
        if (playerShip.load("models/Spaceship/13886_UFO_V1_l2.obj")) {
            // playerShip.overrideTexture(playerTexture);
            GLuint TextID = loadTexture("models/Spaceship/13886_UFO_diffuse.jpg");
            playerShip.overrideTexture(TextID);
            shipLoaded = true;
            playerShip.setScale(0.6f); // ajustar tamanho da nave
        }
    }
}

void reset_player(Player *p){
    p->x = 0.0f; 
    p->y = 0.0f; 
    p->rotation = 0.0f; 
    p->vx = 0.0f;
    p->vy = 0.0f;
    p->lives -= 1;
}

void draw_player(Player *p){
  glPushMatrix();
      glTranslated(p->x,p->y,p->z);
      glRotatef(p->rotation,0.0,0.0,1.0);
      // glRotatef(90.0,-1.0,0.0,0.0);
      // glColor3f(1.0,0.0,1.0);
      // draw_spaceship(p->size);

      // se tiver virado errado, só ajustar aqui
      glRotatef(90.0, 0.0, 0.0, 1.0);
      glRotatef(180.0, 0.0, 1.0, 0.0); // gira 180 graus se a nave tiver de costas

      if (shipLoaded) {
          playerShip.draw();
      } else {
          // desenha a pirâmide se der errado
          glColor3f(1.0, 0.0, 1.0);
          draw_spaceship(p->size);
      }
  glPopMatrix();

}

void draw_spaceship(float size) {
    float height = size * 2.0f;
    float base_half = size / 2.0f;

    float P[3] = {0.0f, 0.0f, height/2};     // Ponta
    float A[3] = { base_half,  base_half, -height/2};
    float B[3] = {-base_half,  base_half, -height/2};
    float C[3] = {-base_half, -base_half, -height/2};
    float D[3] = { base_half, -base_half, -height/2};
    
    glBindTexture(GL_TEXTURE_2D, playerTexture);
    glColor3f(1.0, 1.0, 1.0);

    //BASE
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(C);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(D);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(A);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(B);
    glEnd();

    // LATERAIS 
    // Desenhar as arestas laterais
    glBegin(GL_TRIANGLES);
        // Face Frontal (P-D-A)
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(P);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(D);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(A);

        // Face Direita (P-A-B)
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(P);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(A);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(B);

        // Face Traseira (P-B-C)
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(P);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(B);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(C);

        // Face Esquerda (P-C-D)
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(P);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(C);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(D);
    glEnd();

    if (up) {
        glBindTexture(GL_TEXTURE_2D, propulsorTexture);
        
        float flicker = (rand() % 10) / 20.0f;
        float fireLen = size * (0.5f + flicker);

        float F[3] = {0.0f, 0.0f, -height/2 - fireLen};
        
        glBegin(GL_TRIANGLES);
            
            // Face 1
            glTexCoord2f(0.0f, 0.0f); glVertex3fv(A);
            glTexCoord2f(1.0f, 0.0f); glVertex3fv(B);
            glTexCoord2f(0.5f, 1.0f); glVertex3fv(F);

            // Face 2
            glTexCoord2f(0.0f, 0.0f); glVertex3fv(B);
            glTexCoord2f(1.0f, 0.0f); glVertex3fv(C);
            glTexCoord2f(0.5f, 1.0f); glVertex3fv(F);

            // Face 3
            glTexCoord2f(0.0f, 0.0f); glVertex3fv(C);
            glTexCoord2f(1.0f, 0.0f); glVertex3fv(D);
            glTexCoord2f(0.5f, 1.0f); glVertex3fv(F);

            // Face 4
            glTexCoord2f(0.0f, 0.0f); glVertex3fv(D);
            glTexCoord2f(1.0f, 0.0f); glVertex3fv(A);
            glTexCoord2f(0.5f, 1.0f); glVertex3fv(F);
        glEnd();
        
        // Resetar cor para branco para não afetar outros desenhos
        glColor3f(1.0, 1.0, 1.0);
    }
}

void move_player(Player *p, float delta){
    float accel_amount = p->accel * delta;
    float rotation_rad = p->rotation *(PI/180.0f);

    float frontal_dirx = -sinf(rotation_rad);
    float frontal_diry = cosf(rotation_rad);

    if (up) {
      p->vx += frontal_dirx * accel_amount;
      p->vy += frontal_diry * accel_amount;
    }
    if (rot_left){
      p->rotation += ANGULAR_VEL * delta;
    }
    if(rot_right){
      p->rotation -= ANGULAR_VEL * delta;
    }
    
    p->vx *= p->damping_rate;
    p->vy *= p->damping_rate;
    
    p->x += p->vx * delta;
    p->y += p->vy * delta;

    if (fabs(p->vx) < 0.001f) p->vx = 0.0f;
    if (fabs(p->vy) < 0.001f) p->vy = 0.0f;
    if (p->rotation > 360.0f) p->rotation -= 360.0f;
    if (p->rotation < 0.0f) p->rotation += 360.0f;
    
    const float WRAP_X = 7.0f + 0.5f; //Limite visível + margem
    const float WRAP_Y = 5.2f + 0.5f;
    
    if (p->x > WRAP_X) p->x = -WRAP_X;
    if (p->x < -WRAP_X) p->x = WRAP_X;
    if (p->y > WRAP_Y) p->y = -WRAP_Y;
    if (p->y < -WRAP_Y) p->y = WRAP_Y;
  
}

void player_shot(std::vector<Bullet> &proj,Player *p){

  //checa se ja tem 5 tiros na tela
  if(proj.size() >= MAX_PROJECTILES){return ;}

  //calcula a direção que o player apontando
  float player_angle = (p->rotation)  * (PI/180.0f);
  float shot_dirx = -sinf(player_angle);
  float shot_diry = cosf(player_angle); 
  Bullet new_bullet;

  new_bullet.x = p->x;
  new_bullet.y = p->y;
  new_bullet.z = p->z;
  new_bullet.Vx = shot_dirx * BULLET_SPEED;
  new_bullet.Vy = shot_diry * BULLET_SPEED;

  proj.push_back(new_bullet);
}


void update_bullets(std::vector<Bullet> &proj,float delta){
  for(size_t i = 0; i < proj.size(); ){
    Bullet &b = proj[i];

    b.x += b.Vx * delta;
    b.y += b.Vy * delta;
    b.z += b.Vz * delta;
  
  if(fabs(b.x) > MAX_PRJCT_DISTANCE || fabs(b.y) > MAX_PRJCT_DISTANCE ){
     proj.erase(proj.begin() + i); 
  }else{
      ++i;
    } 
  }
}

void draw_bullet(std::vector<Bullet> &proj){
  for (const auto& bullet : proj){
    glPushMatrix();
      glDisable(GL_TEXTURE_2D);
      glColor3f(0.0f,0.0f,1.0f);
      glTranslated(bullet.x,bullet.y,bullet.z);
      glRotatef(90.0,-1.0,0.0,0.0);
      glPointSize(4.0f);
      glBegin(GL_POINTS);
        glVertex3f(0.0f,0.0f,0.0f);
      glEnd();
      glEnable(GL_TEXTURE_2D);
    glPopMatrix();
  }
}

bool is_player_alive(Player *p){
  return (p->lives >= 0);
}
