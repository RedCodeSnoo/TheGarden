#include "Bird.hpp"

Bird::Bird() : m_position(0.0f, 0.0f, 30.0f), m_rotationAngle(0.0f), m_leftWing(true), m_rightWing(false) {
    m_bodyMesh = nullptr;
    m_headMesh = nullptr;
    m_beakMesh = nullptr;
}

void Bird::init() {
    m_bodyMesh = STP3D::basicSphere(1.0f, 32, 32);
    m_bodyMesh->createVAO();

    m_headMesh = STP3D::basicSphere(0.7f, 24, 24);
    m_headMesh->createVAO();

    m_beakMesh = STP3D::basicCone(1.0f, 0.3f, 0.0f, 16);
    m_beakMesh->createVAO();

    m_leftWing.init();
    m_rightWing.init();
}

void Bird::update(double deltaTime) {
    m_leftWing.update(deltaTime);
    m_rightWing.update(deltaTime);
}

void Bird::draw(GLBI_Engine& engine) {
    if (!m_bodyMesh) return;

    engine.switchToPhongShading();
    engine.mvMatrixStack.pushMatrix();
    
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(m_rotationAngle, 2));

    // DESSIN DU CORPS
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(2.0f, 1.0f, 0.5f));
    engine.setFlatColor(0.2f, 0.4f, 1.0f);
    engine.updateMvMatrix();
    m_bodyMesh->draw();
    engine.mvMatrixStack.popMatrix();

    // DESSIN DE LA TÊTE
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(1.8f, 0.0f, 0.2f)); // Légèrement vers le haut et l'avant
    engine.setFlatColor(0.3f, 0.5f, 1.0f);
    engine.updateMvMatrix();
    m_headMesh->draw();

    // DESSIN DU BEC
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.7f, 0.0f, 0.0f)); 
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(-90.0f), 2)); 
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(1.0f, 1.2f, 1.0f)); 
    engine.setFlatColor(1.0f, 0.8f, 0.0f);
    engine.updateMvMatrix();
    m_beakMesh->draw();
    
    engine.mvMatrixStack.popMatrix(); // Fin bec

    engine.mvMatrixStack.popMatrix(); // Fin tête

    // DESSIN DES AILES
    m_leftWing.draw(engine);
    m_rightWing.draw(engine);

    engine.mvMatrixStack.popMatrix(); // Fin corps
}