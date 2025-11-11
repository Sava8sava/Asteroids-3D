#include "window.h"
#include "game.h"
#include <GL/freeglut.h>
#include <cstdlib>

int main(int argc, char** argv){
    init_window(argc,argv);
    init_game_objs();
    glutMainLoop();
    return EXIT_SUCCESS;
}
