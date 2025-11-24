#ifndef MODEL_H
#define MODEL_H

#include "glm.h" // Inclui a biblioteca GLM fornecida por você

class Model {
public:
    Model();
    ~Model();

    // Carrega o modelo de um arquivo .obj
    // Retorna true se carregou com sucesso
    bool load(const char* filename);

    // Desenha o modelo
    void draw();

    // Define uma escala manual (opcional)
    void setScale(float scale);

    void overrideTexture(GLuint textureID);
private:
    GLMmodel* pModel; // Ponteiro para a estrutura da biblioteca GLM
    float currentScale;
};

#endif