#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "tools/basic_mesh.hpp"
#include "tools/vector3d.hpp"

using namespace glbasimac;
using namespace STP3D;

class Tree {
public:
    Tree(Vector3D position);

    // Initialise les modèles 3D une seule fois pour toute la forêt
    static void initMeshes();
    
    void draw(GLBI_Engine& engine) const;

private:
    Vector3D m_position;

    // Pointeurs partagés par tous les arbres (économie de mémoire)
    static IndexedMesh* s_trunkMesh;
    static IndexedMesh* s_leavesMesh;
};