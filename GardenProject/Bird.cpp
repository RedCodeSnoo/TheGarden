#include "Bird.hpp"

Bird::Bird() : m_position(0.0f, 0.0f, 80.0f), m_rotationAngle(0.0f) {
    m_bodyMesh = nullptr;
}

void Bird::init() {
    m_bodyMesh = STP3D::basicSphere(1.0f, 32, 32);
    m_bodyMesh->createVAO();
}

void Bird::update(double deltaTime) {
    
}

void Bird::draw(GLBI_Engine& engine) {
    if (!m_bodyMesh) return;

    engine.switchToPhongShading();

    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(m_rotationAngle, 2));
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(2.0f, 1.0f, 0.5f));
    engine.setFlatColor(0.2f, 0.4f, 1.0f);
    engine.updateMvMatrix();
    m_bodyMesh->draw();
    engine.mvMatrixStack.popMatrix();
}