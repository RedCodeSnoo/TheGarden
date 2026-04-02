#include "draw_scene.hpp"
#include <fstream>
#include "glbasimac/glbi_texture.hpp"
#include "tools/stb_image.h"

float cameraPositionX {50.0f};
float cameraPositionY {-100.0f};
float cameraPositionZ {50.0f};
float cameraAngle {90.0f};

GLBI_Engine myEngine;
StandardMesh ground;
glbasimac::GLBI_Texture groundTexture;

HeightMap terrainLoader;

void initScene() {

    if (!terrainLoader.load("../assets/textures/terrain.pgm")) {
        std::cerr << "ERROR: Cannot load PGM file ../assets/textures/terrain.pgm" << std::endl;
        return;
    }

    std::vector<float> points;
    std::vector<float> uvs;

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

    for (auto j = 0; j < terrainLoader.getHeight() - 1; j++) {
        for (auto i = 0; i < terrainLoader.getWidth() - 1; i++) {
            
            auto h0 = terrainLoader.getPixel(i, j) * scaleZ;
            auto h1 = terrainLoader.getPixel(i, j + 1) * scaleZ;
            auto h2 = terrainLoader.getPixel(i + 1, j) * scaleZ;
            auto h3 = terrainLoader.getPixel(i + 1, j + 1) * scaleZ;

            // Positions X et Y
            auto x_coord = i * scaleXY;
            auto y_coord = j * scaleXY;
            auto xp1 = (i + 1) * scaleXY;
            auto yp1 = (j + 1) * scaleXY;
            
            // Textures UVs
            auto u0 = (float)i;
            auto v0 = (float)j;
            auto u1 = (float)(i + 1);
            auto v1 = (float)(j + 1);

            // --- Triangle 1 ---
            points.push_back(x_coord);  points.push_back(y_coord);  points.push_back(h0);
            uvs.push_back(u0);          uvs.push_back(v0);
            
            points.push_back(x_coord);  points.push_back(yp1);      points.push_back(h1);
            uvs.push_back(u0);          uvs.push_back(v1);
            
            points.push_back(xp1);      points.push_back(y_coord);  points.push_back(h2);
            uvs.push_back(u1);          uvs.push_back(v0);

            // --- Triangle 2 ---
            points.push_back(xp1);      points.push_back(y_coord);  points.push_back(h2);
            uvs.push_back(u1);          uvs.push_back(v0);
            
            points.push_back(x_coord);  points.push_back(yp1);      points.push_back(h1);
            uvs.push_back(u0);          uvs.push_back(v1);
            
            points.push_back(xp1);      points.push_back(yp1);      points.push_back(h3);
            uvs.push_back(u1);          uvs.push_back(v1);
        }
    }

    ground.setNbElt(points.size() / 3);
    ground.changeType(GL_TRIANGLES);
    ground.addOneBuffer(0, 3, points.data(), "Coordinates", true);
    ground.addOneBuffer(2, 2, uvs.data(), "UVs", true); // Buffer 2 est utilise pour vx_uvs
    if (!ground.createVAO()) {
        std::cerr << "Erreur lors de la creation du VAO de ground" << std::endl;
    }
}


void drawGround() {
    myEngine.setFlatColor(1.0f, 1.0f, 1.0f);

    myEngine.activateTexturing(true);
    groundTexture.attachTexture();
    glActiveTexture(GL_TEXTURE0);

    ground.draw();

    groundTexture.detachTexture();
    myEngine.activateTexturing(false);
}

void drawBird() {
	// TO DO
}

void drawTree() {
	// TO DO
}

void drawStructure() {
	// TO DO
}

void drawScene() {
    a_frame->draw();
    drawGround();
}