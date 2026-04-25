#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"

using namespace glbasimac;
using namespace STP3D;

class Wing {
public:
    Wing(bool isLeft);

    void init();
    void update(double deltaTime);
    void draw(GLBI_Engine& engine);

private:
    bool m_isLeft;
    float m_flapAngle;
    IndexedMesh* m_wingMesh;
    
    float m_flapSpeed;
    float m_maxAngle;
    int m_flapDirection;
};