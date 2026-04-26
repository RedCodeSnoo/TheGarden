#include "Bird.hpp"
#include <cstdlib>

Bird::Bird(Vector3D startPosition, float startYaw) :
    m_leftWing(true), 
    m_rightWing(false),
    m_position(startPosition),
    m_startPos(startPosition),
    m_yaw(startYaw),
    m_pitch(0.0f),
    m_state(0),
    m_targetYaw(0.0f),
    m_totalTime(0.0f),
    m_flySpeed(25.0f),
    m_turnSpeed(1.0f),
    m_baseAltitude(startPosition.z)
    
{
    m_bodyMesh = nullptr;
    m_headMesh = nullptr;
    m_beakMesh = nullptr;

    m_ampZ = 4.0f + (std::rand() % 9);
    m_freqZ = 1.0f + (std::rand() % 13) * 0.1f;
    m_straightDist = 50.0f + (std::rand() % 50);
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
    m_totalTime += deltaTime;

    // modification de la hauteur
    m_position.z = m_baseAltitude + std::sin(m_totalTime * m_freqZ) * m_ampZ;
    m_pitch = std::atan2(m_ampZ * m_freqZ * std::cos(m_totalTime * m_freqZ), m_flySpeed);

    switch (m_state) {
        case 0: { // ligne droite
            auto dist = std::hypot(m_position.x - m_startPos.x, m_position.y - m_startPos.y);
            auto outOfBounds = std::abs(m_position.x) > 90.0f || std::abs(m_position.y) > 90.0f;
            
            if (dist >= m_straightDist || outOfBounds) {
                float turnSign;
                
                if (outOfBounds) {
                    auto centreAGauche = m_position.x * std::sin(m_yaw) - m_position.y * std::cos(m_yaw);
                    turnSign = (centreAGauche > 0.0f) ? 1.0f : -1.0f; 
                } else {
                    turnSign = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
                }

                m_targetYaw = m_yaw + (turnSign * M_PI_2);
                m_state = 1; 
            }
            break;
        }

        case 1: { // virage
            auto diff = std::atan2(std::sin(m_targetYaw - m_yaw), std::cos(m_targetYaw - m_yaw));
            auto step = m_turnSpeed * deltaTime;
            
            if (std::abs(diff) <= step) {
                m_yaw = m_targetYaw;
                m_startPos = m_position;
                m_straightDist = 50.0f + (std::rand() % 50);
                m_state = 0;
            } else {
                m_yaw += (diff > 0 ? step : -step);
            }
            break;
        }
    }
    m_position.x += std::cos(m_yaw) * m_flySpeed * deltaTime;
    m_position.y += std::sin(m_yaw) * m_flySpeed * deltaTime;
}

void Bird::draw(GLBI_Engine& engine) {
    if (!m_bodyMesh) return;

    engine.switchToPhongShading();
    engine.mvMatrixStack.pushMatrix();
    
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(m_yaw, 2));
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(-m_pitch, 1));

    // DESSIN DU CORPS
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::homothety(2.0f, 1.0f, 0.5f));
    engine.setFlatColor(0.2f, 0.4f, 1.0f);
    engine.updateMvMatrix();
    m_bodyMesh->draw();
    engine.mvMatrixStack.popMatrix();

    // DESSIN DE LA TÊTE
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(1.8f, 0.0f, 0.2f));
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