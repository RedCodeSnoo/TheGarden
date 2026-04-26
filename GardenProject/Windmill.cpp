#include "Windmill.hpp"
#include "tools/stb_image.h"
#include <iostream>
#include <cmath>

Windmill::Windmill(Vector3D startPosition) 
    : m_position(startPosition), m_rotationAngle(0.0f), m_rotationSpeed(1.5f),
      m_towerMesh(nullptr), m_roofMesh(nullptr), m_bladeMesh(nullptr) {}

void Windmill::init() {
    m_towerMesh = STP3D::basicCylinder(8.0f, 3.0f, 32, 1);
    m_towerMesh->createVAO();

    m_roofMesh = STP3D::basicCone(4.0f, 3.5f, 0.0f, 16);
    m_roofMesh->createVAO();

    m_bladeMesh = STP3D::basicCube(1.0f);
    m_bladeMesh->createVAO();

    // Chargement des textures
    int x, y, n;
    
    // Texture 1 : Tour
    unsigned char* imageStone = stbi_load("../assets/textures/stone.png", &x, &y, &n, 0);
    if (imageStone) {
        m_stoneTexture.createTexture();
        m_stoneTexture.attachTexture();
        m_stoneTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_stoneTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_stoneTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_stoneTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_stoneTexture.loadImage(x, y, n, imageStone);
        m_stoneTexture.detachTexture();
        stbi_image_free(imageStone);
    } else {
        std::cerr << "ERREUR: Impossible de charger stone.png" << std::endl;
    }

    // Texture 2 : Pales
    unsigned char* imageWood = stbi_load("../assets/textures/wood.png", &x, &y, &n, 0);
    if (imageWood) {
        m_woodTexture.createTexture();
        m_woodTexture.attachTexture();
        m_woodTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_woodTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_woodTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_woodTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_woodTexture.loadImage(x, y, n, imageWood);
        m_woodTexture.detachTexture();
        stbi_image_free(imageWood);
    } else {
        std::cerr << "ERREUR: Impossible de charger wood.png" << std::endl;
    }

    // Texture 3 : Toit
    unsigned char* imageRoof = stbi_load("../assets/textures/roof.png", &x, &y, &n, 0);
    if (imageRoof) {
        m_roofTexture.createTexture();
        m_roofTexture.attachTexture();
        m_roofTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_roofTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_roofTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_roofTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_roofTexture.loadImage(x, y, n, imageRoof);
        m_roofTexture.detachTexture();
        stbi_image_free(imageRoof);
    } else {
        std::cerr << "ERREUR: Impossible de charger roof.png" << std::endl;
    }
}

void Windmill::update(double deltaTime) {
    m_rotationAngle += m_rotationSpeed * deltaTime;
}

void Windmill::draw(GLBI_Engine& engine) {
    if (!m_towerMesh || !m_roofMesh || !m_bladeMesh) return;

    engine.activateTexturing(true);
    engine.mvMatrixStack.pushMatrix();
    
    // Position globale du moulin
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));

    //  Dessin de la Tour (Pierre)
    m_stoneTexture.attachTexture();
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
    engine.setFlatColor(1.0f, 1.0f, 1.0f);
    engine.updateMvMatrix();
    m_towerMesh->draw();
    engine.mvMatrixStack.popMatrix();
    m_stoneTexture.detachTexture();

    //  Dessin du Toit (Tuiles)
    m_roofTexture.attachTexture();
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 8.0f));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
    engine.setFlatColor(1.0f, 1.0f, 1.0f);
    engine.updateMvMatrix();
    m_roofMesh->draw();
    engine.mvMatrixStack.popMatrix();
    m_roofTexture.detachTexture();

    // Dessin des Pales (Bois)
    m_woodTexture.attachTexture();
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, -3.5f, 6.5f));

    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(m_rotationAngle, 1));

    for(int i = 0; i < 4; i++) {
        engine.mvMatrixStack.pushMatrix();
        engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(i * 90.0f), 1));
        engine.mvMatrixStack.addTransformation(Matrix4D::translation(2.0f, 0.0f, 0.0f));
        engine.mvMatrixStack.addTransformation(Matrix4D::homothety(4.0f, 0.1f, 0.5f)); 
        engine.setFlatColor(1.0f, 1.0f, 1.0f);
        engine.updateMvMatrix();
        m_bladeMesh->draw();
        engine.mvMatrixStack.popMatrix();
    }

    engine.mvMatrixStack.popMatrix();
    m_woodTexture.detachTexture();

    engine.mvMatrixStack.popMatrix();
    engine.activateTexturing(false);
}