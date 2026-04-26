#include "Sheep.hpp"
#include <cstdlib>
#include <cmath>

IndexedMesh* Sheep::s_bodyMesh = nullptr;
IndexedMesh* Sheep::s_headMesh = nullptr;

Sheep::Sheep(Vector3D startPos) : m_position(startPos) {
    m_yaw = (std::rand() % 360) * M_PI / 180.0f;
    m_speed = 2.0f + (std::rand() % 10) * 0.1f;
    m_state = 0;
    m_timer = 0.0f;
}

void Sheep::initMeshes() {
    if (!s_bodyMesh) {
        s_bodyMesh = STP3D::basicCube(1.5f); 
        s_bodyMesh->createVAO();
    }
    if (!s_headMesh) {
        s_headMesh = STP3D::basicCube(0.8f); 
        s_headMesh->createVAO();
    }
}

void Sheep::update(double deltaTime) {
    m_timer -= deltaTime;

    if (m_state == 0) {
        m_position.x += std::cos(m_yaw) * m_speed * deltaTime;
        m_position.y += std::sin(m_yaw) * m_speed * deltaTime;
        
        if (m_timer <= 0.0f || std::abs(m_position.x) > 80.0f || std::abs(m_position.y) > 80.0f) {
            m_state = 1;
            m_timer = 1.0f + (std::rand() % 3); 
            m_targetYaw = m_yaw + ((std::rand() % 2 == 0 ? 1 : -1) * (M_PI / 2.0f + (std::rand()%10)*0.1f));
        }
    } else if (m_state == 1) { 
        m_yaw += (m_targetYaw - m_yaw) * deltaTime * 2.0f;
        if (m_timer <= 0.0f) {
            m_state = 0;
            m_timer = 3.0f + (std::rand() % 6); 
        }
    }
}

void Sheep::setZ(float z) {
    m_position.z = z;
}

Vector3D Sheep::getPosition() const {
    return m_position;
}

void Sheep::draw(GLBI_Engine& engine) const {
    if (!s_bodyMesh || !s_headMesh) return;

    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(m_yaw, 2));

    // Laine blanche
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(1.5f, 1.0f, 1.0f));
    engine.setFlatColor(0.9f, 0.9f, 0.9f);
    engine.updateMvMatrix();
    s_bodyMesh->draw();
    engine.mvMatrixStack.popMatrix();

    // Tête noire
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(1.0f, 0.0f, 0.5f));
    engine.setFlatColor(0.2f, 0.2f, 0.2f);
    engine.updateMvMatrix();
    s_headMesh->draw();
    engine.mvMatrixStack.popMatrix();

    engine.mvMatrixStack.popMatrix();
}