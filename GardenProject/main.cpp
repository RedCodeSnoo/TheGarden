#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "draw_scene.hpp"
#include "HeightMap.hpp"
#include "tools/shaders.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "tools/stb_image.h"
#include <iostream>
#include <cmath>

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "Garden Project";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* 3D Engine global variables */
StandardMesh* rectangle;
StandardMesh* a_frame;

GLBI_Convex_2D_Shape cercle;


/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	std::cerr<<"Setting 3D projection"<<std::endl;
	myEngine.set3DProjection(60.0,aspectRatio,Z_NEAR,Z_FAR);
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS); 
    if (is_pressed) {
        switch(key) {
            case GLFW_KEY_ESCAPE :
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_L:
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                break;
            case GLFW_KEY_P:
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                break;
			case GLFW_KEY_F:
				usePhong = !usePhong;
				break;
        }
    }
}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

	}
}

void initBasicScene(const std::string& terrainFilename) {
	a_frame = createRepere(10.0);
	a_frame->createVAO();

	initScene(terrainFilename);

	

}

void renderBasicScene() {
}

int main(int argc, char** argv)
{

	std::string terrainFile = "../assets/textures/terrain.pgm";
    if (argc > 1) {
        terrainFile = argv[1]; // Si un argument est passé, on l'utilise !
    }

	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout<<"Loading GL extension"<<std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	std::cout<<"Engine init"<<std::endl;
	myEngine.mode2D = false; // Set engine to 3D mode
	myEngine.initGL();
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	CHECK_GL;

	initBasicScene(terrainFile);

	double lastTime = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();
		double deltaTime = startTime - lastTime;
		lastTime = startTime;

		float cameraSpeed = 1.0f;
        float turnSpeed = 1.0f;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPositionX += cos(deg2rad(cameraAngle)) * cameraSpeed;
            cameraPositionY += sin(deg2rad(cameraAngle)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPositionX -= cos(deg2rad(cameraAngle)) * cameraSpeed;
            cameraPositionY -= sin(deg2rad(cameraAngle)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            cameraPositionX -= sin(deg2rad(cameraAngle)) * cameraSpeed;
            cameraPositionY += cos(deg2rad(cameraAngle)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            cameraPositionX += sin(deg2rad(cameraAngle)) * cameraSpeed;
            cameraPositionY -= cos(deg2rad(cameraAngle)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            cameraAngle += turnSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cameraAngle -= turnSpeed;
        }
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cameraPositionZ += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraPositionZ -= cameraSpeed;
		}



		updateScene(deltaTime);

		/* Render begins here */
		glClearColor(0.f,0.0f,0.2f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		
		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();

		Vector3D cameraPositionVec = Vector3D(cameraPositionX, cameraPositionY, cameraPositionZ);
		Vector3D targetPoint = Vector3D(
    		cameraPositionX + cos(deg2rad(cameraAngle)),
    		cameraPositionY + sin(deg2rad(cameraAngle)),
    		cameraPositionZ
		);
		Vector3D upVector = Vector3D(0.0, 0.0, 1.0);
		Matrix4D viewMatrix = Matrix4D::lookAt(cameraPositionVec, targetPoint, upVector);
		myEngine.setViewMatrix(viewMatrix);
		myEngine.updateMvMatrix();

		renderBasicScene();
		drawScene();

		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}

	glfwTerminate();
	return 0;
}
