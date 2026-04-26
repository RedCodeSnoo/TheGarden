#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"

using namespace glbasimac;
using namespace STP3D;

class Sheep {
public:
    Sheep(Vector3D startPos);

    static void initMeshes();
    
    void update(double deltaTime);
    void setZ(float z);
    Vector3D getPosition() const;
    void draw(GLBI_Engine& engine) const;

private:
    Vector3D m_position;
    float m_yaw;
    float m_speed;
    float m_targetYaw;
    int m_state; 
    float m_timer;

    static IndexedMesh* s_bodyMesh;
    static IndexedMesh* s_headMesh;
};