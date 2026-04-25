#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"
#include "Wing.hpp"

using namespace glbasimac;
using namespace STP3D;

class Bird {
public:
    Bird();
    
    void init();
    void update(double deltaTime);
    void draw(GLBI_Engine& engine);

private:
    Vector3D m_position;
    float m_rotationAngle;
    
    IndexedMesh* m_bodyMesh;
    IndexedMesh* m_headMesh;
    StandardMesh* m_beakMesh;

    Wing m_leftWing;
    Wing m_rightWing;

    int m_state;
    float m_targetAngle;
    Vector3D m_startLinePosition;
};