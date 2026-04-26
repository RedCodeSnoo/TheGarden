#include "Tree.hpp"

// Définition des variables statiques en dehors de la classe
IndexedMesh* Tree::s_trunkMesh = nullptr;
IndexedMesh* Tree::s_leavesMesh = nullptr;

Tree::Tree(Vector3D position) : m_position(position) {}

void Tree::initMeshes() {
    if (!s_trunkMesh) {
        s_trunkMesh = STP3D::basicCylinder(2.0f, 0.25f, 16, 1);
        s_trunkMesh->createVAO();
    }
    if (!s_leavesMesh) {
        s_leavesMesh = STP3D::basicSphere(1.2f, 16, 16);
        s_leavesMesh->createVAO();
    }
}

void Tree::draw(GLBI_Engine& engine) const {
    if (!s_trunkMesh || !s_leavesMesh) return;

    engine.mvMatrixStack.pushMatrix(); 
    
    // On se place à la base de l'arbre sur le terrain
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(m_position.x, m_position.y, m_position.z));

    // Tronc de l'arbre
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
    engine.setFlatColor(0.35f, 0.20f, 0.05f);
    engine.updateMvMatrix();
    s_trunkMesh->draw();
    engine.mvMatrixStack.popMatrix();

    // Le feuillage central de l'arbre
    engine.mvMatrixStack.pushMatrix();
    engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 2.0f));
    engine.setFlatColor(0.2f, 0.7f, 0.2f);
    engine.updateMvMatrix();
    s_leavesMesh->draw();
    engine.mvMatrixStack.popMatrix();

    // Les branches et leurs feuilles
    int numberOfBranches = 5;
    for (int i = 0; i < numberOfBranches; i++) {
        engine.mvMatrixStack.pushMatrix();

        // Branches
        engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 1.0f));
        float branchAngleZ = i * (360.0f / numberOfBranches);
        engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(branchAngleZ), 2));
        engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(45.0f), 1));

        engine.mvMatrixStack.pushMatrix();
        engine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
        engine.setFlatColor(0.35f, 0.20f, 0.05f);
        engine.updateMvMatrix();
        s_trunkMesh->draw();
        engine.mvMatrixStack.popMatrix();

        // Feuilles
        engine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 2.0f));
        engine.setFlatColor(0.2f, 0.7f, 0.2f);
        engine.updateMvMatrix();
        s_leavesMesh->draw();

        engine.mvMatrixStack.popMatrix();
    }

    engine.mvMatrixStack.popMatrix();
}