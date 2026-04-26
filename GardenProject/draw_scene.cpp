#include "draw_scene.hpp"
#include <fstream>
#include "glbasimac/glbi_texture.hpp"
#include "tools/stb_image.h"
#include "tools/vector3d.hpp"

float cameraPositionX {50.0f};
float cameraPositionY {-100.0f};
float cameraPositionZ {50.0f};
float cameraAngle {90.0f};

GLBI_Engine myEngine;
StandardMesh ground;
glbasimac::GLBI_Texture groundTexture;

HeightMap terrainLoader;
IndexedMesh* sunMesh = nullptr;

// Coordonnées du soleil
float sunPosX = 0.0f;
float sunPosY = 0.0f;
float sunPosZ = 50.0f;

Bird myBird1(Vector3D(10.0f, 10.0f, 30.0f), deg2rad(90.0f));
Bird myBird2(Vector3D(-10.0f, 10.0f, 30.0f), deg2rad(-90.0f));
Bird myBird3(Vector3D(10.0f, -10.0f, 30.0f), deg2rad(0.0f));
Bird myBird4(Vector3D(-10.0f, -10.0f, 30.0f), deg2rad(180.0f));

// Variables globales pour la génération des arbres (positions sur la carte et modèles 3D)
std::vector<Vector3D> treePositions;
IndexedMesh* treeTrunkMesh = nullptr;
IndexedMesh* treeLeavesMesh = nullptr;


void initScene() {

    if (!terrainLoader.load("../assets/textures/terrain.pgm")) {
        std::cerr << "ERROR: Cannot load PGM file ../assets/textures/terrain.pgm" << std::endl;
        return;
    }

    std::vector<float> points;
    std::vector<float> uvs;
    std::vector<float> normals;

    int x = 0, y = 0, n = 0;
    auto image = stbi_load("../assets/textures/herbe.png", &x, &y, &n, 0);
    if (image == nullptr) {
        std::cerr << "ERROR: Cannot load image ../assets/textures/herbe.png" << std::endl;
    } else {
        groundTexture.createTexture();
        groundTexture.attachTexture();
        groundTexture.setParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        groundTexture.setParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        groundTexture.setParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
        groundTexture.setParameters(GL_TEXTURE_WRAP_T, GL_REPEAT);
        groundTexture.loadImage(x, y, n, image);
        groundTexture.detachTexture();
        stbi_image_free(image);
    }

    auto scaleXY = 1.0f; // Distance entre les points
    auto scaleZ = 0.05f; // Facteur d'élévation

    // Calcul de Hmin (la plus petite altitude hors pixel à 0) via la classe
    float hmin = terrainLoader.getMinPixel() * scaleZ;

    // Fonction pour gérer les bords du terrain
    auto get_P = [&](int r_i, int r_j) -> float {
        if (r_i < 0) r_i = 0;
        if (r_i >= terrainLoader.getWidth()) r_i = terrainLoader.getWidth() - 1;
        if (r_j < 0) r_j = 0;
        if (r_j >= terrainLoader.getHeight()) r_j = terrainLoader.getHeight() - 1;
        return terrainLoader.getPixel(r_i, r_j);
    };

    // H(i,j) = P(i,j) * Sh - Hmin
    auto get_H = [&](int r_i, int r_j) -> float {
        return get_P(r_i, r_j) * scaleZ - hmin;
    };

    struct NormalVec { float x, y, z; };
    auto getNormal = [&](int ci, int cj) -> NormalVec {
        float nx = (get_P(ci - 1, cj) - get_P(ci + 1, cj)) * scaleZ / (2.0f * scaleXY);
        float ny = (get_P(ci, cj - 1) - get_P(ci, cj + 1)) * scaleZ / (2.0f * scaleXY);
        float nz = 1.0f;
        float len = std::sqrt(nx*nx + ny*ny + nz*nz);
        return {nx / len, ny / len, nz / len};
    };

    // centrer la carte sur le repere
    float offsetX = (terrainLoader.getWidth() - 1) * scaleXY / 2.0f;
    float offsetY = (terrainLoader.getHeight() - 1) * scaleXY / 2.0f;

    for (auto j = 0; j < terrainLoader.getHeight() - 1; j++) {
        for (auto i = 0; i < terrainLoader.getWidth() - 1; i++) {
            
            auto h0 = get_H(i, j);
            auto h1 = get_H(i, j + 1);
            auto h2 = get_H(i + 1, j);
            auto h3 = get_H(i + 1, j + 1);

            // Positions X et Y centrées
            auto x_coord = i * scaleXY - offsetX;
            auto y_coord = j * scaleXY - offsetY;
            auto xp1 = (i + 1) * scaleXY - offsetX;
            auto yp1 = (j + 1) * scaleXY - offsetY;
            
            // Textures UVs
            auto u0 = (float)i;
            auto v0 = (float)j;
            auto u1 = (float)(i + 1);
            auto v1 = (float)(j + 1);

            // Calculate normals
            auto n00 = getNormal(i, j);
            auto n01 = getNormal(i, j + 1);
            auto n10 = getNormal(i + 1, j);
            auto n11 = getNormal(i + 1, j + 1);

            // On regarde sur la carte d'élévation si cette case contient un arbre
            if (terrainLoader.isTreeLocation(i, j)) {
                treePositions.push_back(Vector3D(x_coord, y_coord, h0));
            }

            // --- Triangle 1 ---
            points.push_back(x_coord);  points.push_back(y_coord);  points.push_back(h0);
            uvs.push_back(u0);          uvs.push_back(v0);
            normals.push_back(n00.x);   normals.push_back(n00.y);   normals.push_back(n00.z);
            
            points.push_back(x_coord);  points.push_back(yp1);      points.push_back(h1);
            uvs.push_back(u0);          uvs.push_back(v1);
            normals.push_back(n01.x);   normals.push_back(n01.y);   normals.push_back(n01.z);
            
            points.push_back(xp1);      points.push_back(y_coord);  points.push_back(h2);
            uvs.push_back(u1);          uvs.push_back(v0);
            normals.push_back(n10.x);   normals.push_back(n10.y);   normals.push_back(n10.z);

            // --- Triangle 2 ---
            points.push_back(xp1);      points.push_back(y_coord);  points.push_back(h2);
            uvs.push_back(u1);          uvs.push_back(v0);
            normals.push_back(n10.x);   normals.push_back(n10.y);   normals.push_back(n10.z);
            
            points.push_back(x_coord);  points.push_back(yp1);      points.push_back(h1);
            uvs.push_back(u0);          uvs.push_back(v1);
            normals.push_back(n01.x);   normals.push_back(n01.y);   normals.push_back(n01.z);
            
            points.push_back(xp1);      points.push_back(yp1);      points.push_back(h3);
            uvs.push_back(u1);          uvs.push_back(v1);
            normals.push_back(n11.x);   normals.push_back(n11.y);   normals.push_back(n11.z);
        }
    }

    ground.setNbElt(points.size() / 3);
    ground.changeType(GL_TRIANGLES);
    ground.addOneBuffer(0, 3, points.data(), "Coordinates", true);
    ground.addOneBuffer(1, 3, normals.data(), "Normals", true);
    ground.addOneBuffer(2, 2, uvs.data(), "UVs", true);
    if (!ground.createVAO()) {
        std::cerr << "Erreur lors de la creation du VAO de ground" << std::endl;
    }

    sunMesh = STP3D::basicSphere(5.0f, 32, 32);
    if (!sunMesh->createVAO()) {
        std::cerr << "Erreur lors de la creation du VAO de sunMesh" << std::endl;
    }

    myEngine.switchToPhongShading();
    myEngine.setLightPosition(Vector4D{sunPosX, sunPosY, sunPosZ, 1.0}); 
	myEngine.setLightIntensity(Vector3D{1.0f, 1.0f, 1.0f}); 
    
    myEngine.setAttenuationFactor(Vector3D{1.0, 0.0, 0.0});  
    
	myEngine.switchToFlatShading();

    // Tronc
    treeTrunkMesh = STP3D::basicCylinder(2.0f, 0.25f, 16, 1);
    treeTrunkMesh->createVAO();
    
    // Feuilles
    treeLeavesMesh = STP3D::basicSphere(1.2f, 16, 16);
    treeLeavesMesh->createVAO();

    myBird1.init();
    myBird2.init();
    myBird3.init();
    myBird4.init();
}



void drawGround() {
    myEngine.switchToPhongShading();
    myEngine.setFlatColor(1.0f, 1.0f, 1.0f);

    myEngine.activateTexturing(true);
    groundTexture.attachTexture();
    glActiveTexture(GL_TEXTURE0);

    myEngine.updateMvMatrix();
    ground.draw();

    groundTexture.detachTexture();
    myEngine.activateTexturing(false);
    myEngine.switchToFlatShading();
}

void drawSun() {
    myEngine.mvMatrixStack.pushMatrix();
    myEngine.mvMatrixStack.addTransformation(Matrix4D::translation(sunPosX, sunPosY, sunPosZ));
    myEngine.setFlatColor(1.0f, 1.0f, 0.0f);
    myEngine.updateMvMatrix();
    sunMesh->draw();
    myEngine.mvMatrixStack.popMatrix();
}

void drawBird() {
	myBird1.draw(myEngine);
    myBird2.draw(myEngine);
    myBird3.draw(myEngine);
    myBird4.draw(myEngine);
}

void drawTree() {
    myEngine.switchToFlatShading(); 
    
    for (const auto& pos : treePositions) {
        myEngine.mvMatrixStack.pushMatrix(); 
        
        // On se place à la base de l'arbre sur le terrain
        myEngine.mvMatrixStack.addTransformation(Matrix4D::translation(pos.x, pos.y, pos.z));

        // Tronc de l'arbre
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
        myEngine.setFlatColor(0.35f, 0.20f, 0.05f);
        myEngine.updateMvMatrix();
        treeTrunkMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        // Le feuillage central de l'arbre
        myEngine.mvMatrixStack.pushMatrix();
        myEngine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 2.0f));
        myEngine.setFlatColor(0.2f, 0.7f, 0.2f);
        myEngine.updateMvMatrix();
        treeLeavesMesh->draw();
        myEngine.mvMatrixStack.popMatrix();

        // Les branches et leurs feuilles
        int numberOfBranches = 5;
        for (int i = 0; i < numberOfBranches; i++) {
            myEngine.mvMatrixStack.pushMatrix();

            // Branches
            myEngine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 1.0f));
            float branchAngleZ = i * (360.0f / numberOfBranches);
            myEngine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(branchAngleZ), 2));
            myEngine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(45.0f), 1));

            myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addTransformation(Matrix4D::rotation(deg2rad(90.0f), 0));
            myEngine.setFlatColor(0.35f, 0.20f, 0.05f);
            myEngine.updateMvMatrix();
            treeTrunkMesh->draw();
            myEngine.mvMatrixStack.popMatrix();

            // Feuilles
            myEngine.mvMatrixStack.addTransformation(Matrix4D::translation(0.0f, 0.0f, 2.0f));
            myEngine.setFlatColor(0.2f, 0.7f, 0.2f);
            myEngine.updateMvMatrix();
            treeLeavesMesh->draw();

            myEngine.mvMatrixStack.popMatrix();
        }

        myEngine.mvMatrixStack.popMatrix();
    }
}

void drawStructure() {
	// TO DO
}

void drawScene() {
    a_frame->draw();
    drawGround();
    drawSun();
    drawTree();
    drawBird();
}

void updateScene(double deltaTime) {
    myBird1.update(deltaTime);
    myBird2.update(deltaTime);
    myBird3.update(deltaTime);
    myBird4.update(deltaTime);
}