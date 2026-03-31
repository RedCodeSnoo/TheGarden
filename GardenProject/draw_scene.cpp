#include "draw_scene.hpp"
#include <fstream>

float cameraPositionX {50.0f};
float cameraPositionY {-100.0f};
float cameraPositionZ {50.0f};
float cameraAngle {90.0f};

GLBI_Engine myEngine;
GLBI_Convex_2D_Shape ground{3};

HeightMap terrainLoader;


void initScene() {

    if (!terrainLoader.load("../assets/textures/terrain.pgm")) {
        std::cerr << "Erreur : Impossible de charger le fichier PGM !" << std::endl;
        return;
    }

    std::vector<float> points;

    auto scaleXY = 1.0f; // Distance entre les points
    auto scaleZ = 0.05f; // Facteur d'élévation

    for (auto j = 0; j < terrainLoader.getHeight() - 1; j++) {
        for (auto i = 0; i < terrainLoader.getWidth() - 1; i++) {
            
            auto h0 = terrainLoader.getPixel(i, j) * scaleZ;
            auto h1 = terrainLoader.getPixel(i, j + 1) * scaleZ;
            auto h2 = terrainLoader.getPixel(i + 1, j) * scaleZ;
            auto h3 = terrainLoader.getPixel(i + 1, j + 1) * scaleZ;

            // Positions X et Y
            auto x = i * scaleXY;
            auto y = j * scaleXY;
            auto xp1 = (i + 1) * scaleXY;
            auto yp1 = (j + 1) * scaleXY;

            // --- Triangle 1 ---
            points.push_back(x);   points.push_back(y);   points.push_back(h0);
            points.push_back(x);   points.push_back(yp1); points.push_back(h1);
            points.push_back(xp1); points.push_back(y);   points.push_back(h2);

            // --- Triangle 2 ---
            points.push_back(xp1); points.push_back(y);   points.push_back(h2);
            points.push_back(x);   points.push_back(yp1); points.push_back(h1);
            points.push_back(xp1); points.push_back(yp1); points.push_back(h3);
        }
    }

    
    ground.initShape(points);
    ground.changeNature(GL_TRIANGLES);
}


void drawGround() {
    myEngine.setFlatColor(0.2f, 0.6f, 0.2f); 
    ground.drawShape();
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