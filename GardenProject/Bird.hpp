#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"
#include "Wing.hpp"

using namespace glbasimac;
using namespace STP3D;

class Bird {
public:
    Bird(Vector3D startPosition = Vector3D(0.0f, 0.0f, 30.0f), float startYaw = 0.0f);
    
    void init();
    void update(double deltaTime);
    void draw(GLBI_Engine& engine);

    Vector3D getPosition() const { return m_position; }

private:
    IndexedMesh* m_bodyMesh;
    IndexedMesh* m_headMesh;
    StandardMesh* m_beakMesh;
    Wing m_leftWing;
    Wing m_rightWing;

    Vector3D m_position;
    Vector3D m_startPos;
    float m_yaw;
    float m_pitch;

    int m_state;
    float m_targetYaw;
    float m_totalTime;

    float m_flySpeed;
    float m_turnSpeed;
    float m_straightDist;
    float m_ampZ;
    float m_freqZ;
    const float m_baseAltitude;
};