#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

typedef struct{
  float x,y,z;
  float dx,dy;
  float velocity;
  float size;
  uint8_t type;
  float shoot_timer;
  float shoot_interval;
  bool active;
}Ufo;

enum Ufos_types{BIG_UFO = 0,LIL_UFO};

typedef struct{
    float x,y,z;
    float rotation;
    float size;
    float vx,vy;
    float accel; 
    float damping_rate;
    int8_t lives; //inteiro de um byte
} Player; 

typedef struct{
    float x,y,z;
    float Vx,Vy,Vz; 
} Bullet;

//variaveis para os menus 
enum Gamestates{MENU = 0, PLAYING, GAME_OVER};
//constantes
const float ANGULAR_VEL = 360.0f;
const float PI = 3.1415926f;
const float BOUNDARY_X = 8.0f;
const float BOUNDARY_Y = 6.0f;


#endif
