#ifndef GAME_H
#define GAME_H 


//Global variables 
typedef struct{
    float x,y,z;
    float rotation;
    float size;
    float vx,vy;
    float accel; 
    float damping_rate;
} Entity; 

typedef struct{
    float x,y,z;
    float Vx,Vy,Vz;
} Bullet;


//gameloop
void init_game_objs();
void fps_counter();
void calculate_delta();

//ações do meteoro 
void init_desenhoMeteoro();

//ações do player 
//global player action variables 
extern bool up,rot_left,rot_right,spacekey;
void move_player();
void draw_spaceship(float size);
void player_shot(Entity *p);
void update_bullets();
void draw_bullet();
#endif
