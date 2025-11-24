#include "model.h"
#include <cstdio>
#include <GL/freeglut.h>

Model::Model() {
    pModel = NULL;
    currentScale = 1.0f;
}

Model::~Model() {
    if (pModel) {
        glmDelete(pModel); //limpa a memória usando a função da lib
        pModel = NULL;
    }
}

bool Model::load(const char* filename) {
    if (pModel) {
        glmDelete(pModel);
        pModel = NULL;
    }
    
    // glmReadOBJ carrega a geometria e materiais
    // O segundo parâmetro indica para ler materiais mtl se existirem
    pModel = glmReadOBJ((char*)filename, true);
    
    if (!pModel) {
        printf("ERRO: Nao foi possivel carregar o modelo: %s\n", filename);
        return false;
    }

    // glmUnitize: Redimensiona o modelo para caber em um cubo 1x1x1 unitário.
    // Isso garante que a nave não fique gigante nem minúscula.
    glmUnitize(pModel);
    
    // Gera normais para as faces
    glmFacetNormals(pModel);
    
    // Gera normais para os vértices
    glmVertexNormals(pModel, 90.0f);
    
    return true;
}

void Model::draw() {
    if (!pModel) return;

    glPushMatrix();
    
    // Aplica a escala definida
    glScalef(currentScale, currentScale, currentScale);
    
    // glmDraw faz o desenho efetivo.
    // GLM_SMOOTH: Usa normais de vértices (suaviza a malha)
    // GLM_TEXTURE: Usa coordenadas de textura (se o modelo tiver UVs)
    // GLM_MATERIAL: Usa os materiais definidos no mtl (cores, texturas)
    glmDraw(pModel, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
    
    glPopMatrix();
}

void Model::overrideTexture(GLuint textureID) {
    if (!pModel || !pModel->materials) return;

    // Percorre todos os materiais definidos no modelo e troca a textura
    for (GLuint i = 0; i < pModel->nummaterials; i++) {
        pModel->materials[i].texture = textureID;
    }
}

void Model::setScale(float scale) {
    this->currentScale = scale;
}