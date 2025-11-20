#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <GL/glut.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

GLuint playerTexture;
GLuint meteorTexture;
GLuint propulsorTexture;
GLuint backTexture;

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configurações de filtro (Importante: use MIPMAP no MinFilter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Suavização de alta qualidade
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    if (data) {
        int format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);

    } else {
        printf("Falha ao carregar textura: %s\n", filename);
    }
    
    stbi_image_free(data);
    return textureID;
}