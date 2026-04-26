#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"

using namespace glbasimac;
using namespace STP3D;

class Windmill {
public:
    Windmill(Vector3D startPosition = Vector3D(10.0f, 10.0f, 0.0f));

    void init();
    void update(double deltaTime);
    void draw(GLBI_Engine& engine);

private:
    Vector3D m_position;
    float m_rotationAngle;

    IndexedMesh* m_towerMesh;
    StandardMesh* m_roofMesh;
    IndexedMesh* m_bladeMesh;

    GLBI_Texture m_stoneTexture;
    GLBI_Texture m_woodTexture;
    GLBI_Texture m_roofTexture;
};