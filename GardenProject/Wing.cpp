#include "Wing.hpp"
#include <cmath>

Wing::Wing(bool isLeft) 
    : m_isLeft(isLeft), m_flapAngle(0.0f), m_wingMesh(nullptr), 
      m_flapSpeed(3.0f), m_maxAngle(0.6f), m_flapDirection(1) {}

void Wing::init() {
    m_wingMesh = STP3D::basicSphere(1.0f, 16, 16);
    m_wingMesh->createVAO();
}

void Wing::update(double deltaTime) {
    m_flapAngle += m_flapSpeed * deltaTime * m_flapDirection;

    if (m_flapAngle >= m_maxAngle) {
        m_flapAngle = m_maxAngle;
        m_flapDirection = -1;
    } 
    else if (m_flapAngle <= -m_maxAngle) {
        m_flapAngle = -m_maxAngle;
        m_flapDirection = 1;
    }
}

void Wing::draw(GLBI_Engine& engine) {
    if (!m_wingMesh) return;

    auto sideOffset = m_isLeft ? -0.8f : 0.8f;
    auto currentAngle = m_isLeft ? -m_flapAngle : m_flapAngle;
    auto pivotOffset = m_isLeft ? -2.0f : 2.0f;
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, sideOffset, 0.0f));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(currentAngle, 0)); 
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, pivotOffset, 0.0f));
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(0.5f, 2.0f, 0.1f));
    engine.updateMvMatrix();

    m_wingMesh->draw();
    engine.mvMatrixStack.popMatrix();
}