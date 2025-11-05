#include "window.h"
#include <GL/freeglut_std.h>
#include <cstdlib>

int main(int argc, char** argv){
    init_window(argc,argv);
    glutMainLoop();
    return EXIT_SUCCESS;
}
