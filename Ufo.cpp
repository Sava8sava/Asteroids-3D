
#include <stdlib.h>
#include <cmath>
#include "Ufo.h"
#include "types.h"
#include "model.h"
#include "texture.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>

const float BIG_UFO_VELOCITY = 3.0f;
const float BIG_UFO_SIZE = 0.8f;
const float LIL_UFO_VELOCIY = 7.0f; 
const float LIL_UFO_SIZE = 0.3f;
const float BULLET_SPEED = 2.0f;
const float MAX_PRJCT_DISTANCE = 10.0f; 
const int MAX_PROJECTILES = 3;

Model ufoModel;
bool isUfoLoaded = false;

//gambiarra,ajeitar depois
float _randRange(float min, float max) {
  return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
}

Ufos_types ufo_spawn_prob(int points){
  // de 30k pra baixo apenas mini ufos aparecem 
  if (points < 30000){
    const int LIL_PROB = 10;
    int roll = rand() % 100;
    if (roll < LIL_PROB){
      return LIL_UFO;
    }else{
      return BIG_UFO; 
    }
  }else{ 
    return LIL_UFO;
  }
}

void init_ufo(Ufo *u, int points){
  u->type = ufo_spawn_prob(points);
  u->shoot_timer = 0.0f;
  u->shoot_interval = 5.0f;
  if(u->type == 0){
    u->size = BIG_UFO_SIZE;
    u->velocity = BIG_UFO_VELOCITY;
  }else{
    u->size = LIL_UFO_SIZE;
    u->velocity = LIL_UFO_VELOCIY;
  }
  
  if (rand() % 2 == 0){
      u->x = (rand() % 2 == 0) ? BOUNDARY_X + 2.0f : -BOUNDARY_X - 2.0f;
      u->y = _randRange(-BOUNDARY_Y, BOUNDARY_Y);
    } else {
      u->y = (rand() % 2 == 0) ? BOUNDARY_Y + 2.0f : -BOUNDARY_Y - 2.0f;
      u->x = _randRange(-BOUNDARY_X, BOUNDARY_X);
  }
  
  int random_angle = rand() % 360;
  float move_angle = (float)random_angle * (PI/180.0f);
  u->dx = -sinf(move_angle);
  u->dy = cosf(move_angle);

  u->active = true;
}

void update_ufo(Ufo *u,float delta){
  const float WRAP_X = BOUNDARY_X + 4.0f;
  const float WRAP_Y = BOUNDARY_Y + 4.0f;
  const float DESPAWN_LIMIT_X = BOUNDARY_X + 10.0f;
  const float DESPAWN_LIMIT_Y = BOUNDARY_Y + 10.0f; 
  
  if (fabs(u->x) > DESPAWN_LIMIT_X || fabs(u->y) > DESPAWN_LIMIT_Y) {
      u->active = false;
      return;
  }

  u->x += u->velocity * u->dx * delta;
  u->y += u->velocity * u->dy * delta;  
  if (u->x > WRAP_X){ u->x = -WRAP_X;}
  if (u->x < -WRAP_X){ u->x = WRAP_X;}
  if (u->y > WRAP_Y){ u->y = -WRAP_Y;}
  if (u->y < -WRAP_Y){ u->y = WRAP_Y;}
}

void draw_ufo(Ufo *u){
  if(!u->active){ return; }

  if (!isUfoLoaded) {
      // COLOQUE O ARQUIVO 'ufo.obj' NA PASTA CORRETA
      if (ufoModel.load("models/Round_50s_Flying_Saucer_v1_L3.123cfa4a1570-edd0-4f74-8357-4d26f62cc3a1/10476_Round_50s_Flying_Saucer_v1_L3.obj")) {
          isUfoLoaded = true;
          ufoModel.overrideTexture(ufoTexture);
          printf("Modelo UFO carregado!\n");
      } else {
          // Se falhar, avisa no console apenas uma vez para não spammar
          bool warned = false;
          if(!warned) { printf("Falha ao carregar ufo.obj, usando cubo.\n"); warned = true; }
      }
  }

  glPushMatrix();
    glTranslated(u->x,u->y,u->z);
    // glRotated(90.0,-1.0,0.0,0.0);
    glRotated(180.0,-1.0,0.0,0.0);

    if (isUfoLoaded) {
        float scale = u->size;
        glScalef(scale, scale, scale);

        glColor3f(1.0f, 1.0f, 1.0f); // Branco para não tingir a textura original do modelo
        ufoModel.draw();
    } else {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.0, 1.0, 0.5);
        glutSolidCube(u->size);
        glEnable(GL_TEXTURE_2D);
    }
  glPopMatrix();

}

void spawn_ufo(Ufo *u,float &ufo_time, int points){
  if (!u->active){
    //tenta spwanar uma nave a cada 5 segundos
    if(ufo_time >= 5.0f){
      const int SPAWN_CHANCE = 50;//6%
      if(rand() % 100 < SPAWN_CHANCE){
        init_ufo(u,points);
      }
      ufo_time = 0.0f;
    }
  } 
}

void ufo_shot(std::vector<Bullet> &ufo_proj, Ufo *u, Player *p){
  if(ufo_proj.size() >= MAX_PROJECTILES){return ;}
  
  float shot_dirx = 0;
  float shot_diry = 0;
  
  //se o ufo for do tipo grande atira aleatoriamente 
  if(u->type == BIG_UFO){
    int random_angle = rand() % 360;
    float Bufo_angle = (float)random_angle * (PI/180.0f);
    shot_dirx = -sinf(Bufo_angle);
    shot_diry = cosf(Bufo_angle);
  }else{
    //algebra linear desgraça 
    float dx = p->x - u->x;
    float dy = p->y - u->y;
    
    //normaliza o vetor
    float len = sqrtf(dx * dx + dy * dy);

    shot_dirx = dx / len;
    shot_diry = dy / len;
    }
  
  Bullet new_ufo_bullet;
  new_ufo_bullet.x = u->x;
  new_ufo_bullet.y = u->y;
  new_ufo_bullet.z = u->z;
  new_ufo_bullet.Vx = shot_dirx * BULLET_SPEED;
  new_ufo_bullet.Vy = shot_diry * BULLET_SPEED;

  ufo_proj.push_back(new_ufo_bullet);
}

void update_ufo_bullets(std::vector<Bullet> &ufo_proj,float delta){
  for(size_t i = 0; i < ufo_proj.size(); ){
    Bullet &b = ufo_proj[i];

    b.x += b.Vx * delta;
    b.y += b.Vy * delta;
    b.z += b.Vz * delta;
  
  if(fabs(b.x) > MAX_PRJCT_DISTANCE || fabs(b.y) > MAX_PRJCT_DISTANCE ){
     ufo_proj.erase(ufo_proj.begin() + i); 
  }else{
      ++i;
    } 
  }
}

void draw_ufo_bullet(std::vector<Bullet> &ufo_proj){
  for (const auto& bullet : ufo_proj){
    glPushMatrix();
      glDisable(GL_TEXTURE_2D);
      glColor3f(1.0f,0.0f,0.0f);
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
