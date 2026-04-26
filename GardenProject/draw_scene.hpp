#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "HeightMap.hpp"
#include "Bird.hpp"
#include "Windmill.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR {0.1f};
static const float Z_FAR {500.f};

extern float cameraPositionX;
extern float cameraPositionY;
extern float cameraPositionZ;
extern float cameraAngle;

/* OpenGL Engine */
extern GLBI_Engine myEngine;
extern GLBI_Set_Of_Points somePoints;
extern GLBI_Convex_2D_Shape cercle;
extern StandardMesh* a_frame;

extern Bird myBird;
extern Windmill myWindmill;

extern bool usePhong;

void initScene();

void drawFrame();

void drawGround();

void drawBird();

void drawTree();

void drawStructure();

void drawScene();

void updateScene(double deltaTime);