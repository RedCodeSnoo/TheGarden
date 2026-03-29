#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"

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

void initScene();

void drawFrame();

void drawBase();

void drawArm();

void drawPan();

void drawScene();