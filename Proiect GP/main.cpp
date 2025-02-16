//
//  main.cpp
//  OpenGL_Shader_Example_step1
//
//  Created by CGIS on 02/11/16.
//  Copyright   2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream> // for files
#include <sstream>
#include <chrono>
#include <random>


#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"


unsigned int NUM_SNOWFLAKES = 6000;



int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

glm::mat4 model;
GLint modelLoc;
GLuint quadVAO, quadVBO;


gps::Camera myCamera(glm::vec3(0.46233f, 1.93777f, -9.59824f), glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));


bool pressedKeys[1024];
float angle = 0.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool wireframeMode = false;
bool pointMode = false;
bool turnLights = true;
bool automatedTour = false;

float fov = 45.0f;





gps::Model3D myModel, myModel2, myModel3, myModel4, lamp, celestial, podea, felinar, lirchuchici, goofy[60],bottle1,bottle2, bottle3_1, bottle3_2;
gps::Shader skyShader, snowShader, fogShader, fogShaderObj, lightHouse, combined, screenFrame0, screenFrame1, shaderBlur;
const float movementThreshold = 10.0f;
glm::vec3 previousCamPos = myCamera.getCameraPos();

//for the postProcessing :0

GLuint quadVAOScreen, quadVBOScreen;
GLuint framebuffer[2];
GLuint textureColorbuffer[2];
GLuint rbo[2];
bool horizontal = true, first_iteration = true;

const int NUM_LIGHTS = 2;
GLfloat lightAngle;



// Initialize arrays for light source properties
glm::vec3 lightPosEye[NUM_LIGHTS + 1] = {
	glm::vec3(0.0f),
	glm::vec3(0.0f),
	glm::vec3(0.0f)
};

glm::vec3 lightColor[NUM_LIGHTS + 1] = {
	glm::vec3(0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
};

glm::vec3 cameraPosEye[NUM_LIGHTS + 1] = {
	glm::vec3(0.0f),
	glm::vec3(0.0f),
	glm::vec3(0.0f)
};

float ambientStrength[NUM_LIGHTS + 1] = {
	0.0f,
	6.2f,
	6.2f
};

float specularStrength[NUM_LIGHTS + 1] = {
	0.0f,
	1.5f,
	1.5f
};

float shininess[NUM_LIGHTS + 1] = {
	0.0f,
	1.0f,
	1.0f
};

float constant[NUM_LIGHTS + 1] = {
	0.0f,
	1.0f,
	1.0f
};

float linear[NUM_LIGHTS + 1] = {
	0.0f,
	0.35f,
	0.35f
};

float quadratic[NUM_LIGHTS + 1] = {
	0.0f,
	2.8f,
	2.8f
};


float quadVertices[] = {
	// pozitii de cadrane
	-1.0f,  1.0f, // top-left
	-1.0f, -1.0f, // bottom-left
	 1.0f, -1.0f, // bottom-right
	 1.0f,  1.0f  // top-right
};



float quadScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};


GLfloat vertices[] = {
	// Positions          // Colors (set to #DCDFF3)
	-0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //0
	 0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //1
	 0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //2
	-0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //3
	-0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //4
	 0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //5
	 0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //6
	-0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //7
	-0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //8
	 0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //9
	 0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //10
	-0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //11
	-0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //12
	 0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //13
	 0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //14
	-0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //15
	-0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //16
	 0.5f, -0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //17
	 0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //18
	-0.5f, -0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //19
	-0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //20
	 0.5f,  0.5f, -0.5f, 0.8627f, 0.8745f, 0.9529f, //21
	 0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f, //22
	-0.5f,  0.5f,  0.5f, 0.8627f, 0.8745f, 0.9529f  //23
};

GLuint indices[] = {
	0, 1, 2, 0, 2, 3, // bottom plane triangles
	8, 9, 5, 8, 5, 4, // back plane triangles
	17, 10, 6, 17, 6, 13, // right plane triangles
	16, 12, 7, 16, 7, 11, // left plane triangles
	20, 21, 14, 20, 14, 15, // top plane triangles
	19, 18, 22, 19, 22, 23 // front plane triangles
};

GLuint snowflakeVBO;
GLuint snowflakeEBO;
GLuint snowflakeVAO;

struct Snowflake {
	glm::vec3 position;
	glm::vec3 velocity;
};

std::vector<Snowflake> snowflakes;



//Astea pentru shadow Mapping
glm::mat4 lightRotation;
gps::Model3D lightCube;
gps::Model3D screenQuad, casa;
gps::Shader depthMapShader;
const unsigned int SHADOW_WIDTH = 6* 2048;
const unsigned int SHADOW_HEIGHT = 6* 2048;
GLuint shadowMapFBO;
gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
glm::vec3 lightDir;
glm::mat3 normalMatrix;
glm::vec3 lightColor2;
GLuint depthMapTexture;
GLint lightDirLoc;
GLuint viewLoc;
//Astea pentru shadow Mapping




//--------------------------------------------------- for storm
bool stormMode = false;
float stormTimer = 0.0f;
float stormElapsed = 0.0f;
float stormDuration = 5.0f;
float lightningTimer = 0.0f;
float lightningInterval = 0.0f;
float thunderDuration = 0.3f;
bool isThundering = false;
std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
std::uniform_real_distribution<float> speed_distribution(0.0f, 2.5f);
std::uniform_real_distribution<float> lightningDistribution(0.0f, 15.0f);
//--------------------------------------------------- for storm


void setUniformsCombined(
	gps::Shader shader,
	glm::vec3 fogColor = glm::vec3(0.9, 0.9, 0.9),
	float density = 0.1,
	float exponent = 8,
	float ambientStrengthGlobal = 2.2f,
	float specularStrengthGlobal = 5.2f,
	float shininessGlobal = 2.0f
) {
	shader.useShaderProgram();

	// Set the model matrix

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = myCamera.getViewMatrix();
	GLint viewLoc2 = glGetUniformLocation(shader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	GLint projLoc2 = glGetUniformLocation(shader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));
	//--------------------------------------------------------------------------------------------------------------------------

	// Set the normal matrix
	glm::mat3 normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	GLint normalMatrixLoc = glGetUniformLocation(shader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


	// --- For the point lighting ---
	for (int i = 1; i <= 2; i++)
	{
		std::string lightSrcString = "lightSource";
		std::string index = std::to_string(i);
		lightSrcString = lightSrcString + index;


		GLint lightPosEyeLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".lightPosEye").c_str());
		glUniform3fv(lightPosEyeLoc, 1, glm::value_ptr(lightPosEye[i]));

		GLint lightColorLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".lightColor").c_str());
		glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor[i]));

		GLint cameraPosEyeLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".cameraPosEye").c_str());
		glUniform3fv(cameraPosEyeLoc, 1, glm::value_ptr(cameraPosEye[i]));

		GLint ambientStrengthLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".ambientStrength").c_str());
		glUniform1f(ambientStrengthLoc, ambientStrength[i]);

		GLint specularStrengthLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".specularStrength").c_str());
		glUniform1f(specularStrengthLoc, specularStrength[i]);

		GLint shininessLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".shininess").c_str());
		glUniform1f(shininessLoc, shininess[i]);

		GLint constantLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".constant").c_str());
		glUniform1f(constantLoc, constant[i]);

		GLint linearLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".linear").c_str());
		glUniform1f(linearLoc, linear[i]);

		GLint quadraticLoc = glGetUniformLocation(shader.shaderProgram, (lightSrcString + ".quadratic").c_str());
		glUniform1f(quadraticLoc, quadratic[i]);
	}



	GLint fogColorLoc = glGetUniformLocation(shader.shaderProgram, "fogColor");
	glUniform3fv(fogColorLoc, 1, glm::value_ptr(fogColor));

	GLint densityLoc = glGetUniformLocation(shader.shaderProgram, "density");
	glUniform1f(densityLoc, density);

	GLint exponentLoc = glGetUniformLocation(shader.shaderProgram, "exponent");
	glUniform1f(exponentLoc, exponent);


	GLint ambientStrengthLocGlobal = glGetUniformLocation(shader.shaderProgram, "ambientStrength");
	glUniform1f(ambientStrengthLocGlobal, ambientStrengthGlobal);

	GLint specularStrengthLocGlobal = glGetUniformLocation(shader.shaderProgram, "specularStrength");
	glUniform1f(specularStrengthLocGlobal, specularStrengthGlobal);

	GLint shininessLocGlobal = glGetUniformLocation(shader.shaderProgram, "shininess");
	glUniform1f(shininessLocGlobal, shininessGlobal);
}



void checkFramebufferStatus() {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_COMPLETE:
		std::cout << "Framebuffer is complete." << std::endl;
		break;
	case GL_FRAMEBUFFER_UNDEFINED:
		std::cout << "Framebuffer is undefined." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "Framebuffer has incomplete attachment." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "Framebuffer is missing attachment." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "Framebuffer has incomplete draw buffer." << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "Framebuffer has incomplete read buffer." << std::endl;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "Framebuffer is unsupported." << std::endl;
		break;
	default:
		std::cout << "Framebuffer has unknown error." << std::endl;
		break;
	}
}

void initSnowflakes() {
	glm::vec3 camPos = myCamera.getCameraPos();
	for (int i = 0; i < NUM_SNOWFLAKES; i++) {
		Snowflake snowflake;
		snowflake.position = glm::vec3(
			camPos.x + (float)(rand() % 40 - 20),  // random x within [-20, 20]
			camPos.y + (float)(rand() % 40 - 20),  // random y within [-20, 20]
			camPos.z + (float)(rand() % 40 - 20) // random y within [-20, 20]
		);
		snowflake.velocity = glm::vec3(0.0f, -0.1f, 0.0f);  // falling downwards
		snowflakes.push_back(snowflake);
	}
}


void updateSnowflakes() {
	glm::vec3 currentCamPos = myCamera.getCameraPos();
	glm::vec3 camMovement = currentCamPos - previousCamPos;

	for (auto& snowflake : snowflakes) {
		snowflake.position += snowflake.velocity * glm::vec3(0.02, 0.02, 0.02);
		int random = rand()%3 + 1;
		 if (stormMode) {
            snowflake.position.x -= (2.5f - (float)random) * 0.1f; // add lateral movement
        }

		snowflake.position.y += camMovement.y;
		float distanceX = abs(currentCamPos.x - snowflake.position.x);
		float distanceZ = abs(currentCamPos.z - snowflake.position.z);

		// reset position if it goes out of bounds
		if (snowflake.position.y < currentCamPos.y - 5.0f || snowflake.position.y > currentCamPos.y + 20.0f || distanceX >= 10.0f || distanceZ >= 10.0f) {
			snowflake.position.y = currentCamPos.y + (float)(rand() % 40 - 20);  // reset position within [-20, 20]
			snowflake.position.x = currentCamPos.x + (float)(rand() % 40 - 20);  // random x within [-20, 20]
			snowflake.position.z = currentCamPos.z + (float)(rand() % 40 - 20);  // random z within [-20, 20]
		}
	}
	previousCamPos = currentCamPos;
}


void initSnowFlakeData() {
	glGenVertexArrays(1, &snowflakeVAO);
	glBindVertexArray(snowflakeVAO);

	glGenBuffers(1, &snowflakeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, snowflakeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &snowflakeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snowflakeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}



void initQuadScreenVertices() {
	glGenVertexArrays(1, &quadVAOScreen);

	glGenBuffers(1, &quadVBOScreen);
	glBindVertexArray(quadVAOScreen);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBOScreen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadScreenVertices), &quadScreenVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}


void renderSnowflakes() {
	for (const auto& snowflake : snowflakes) {
		snowShader.useShaderProgram();
		fogShader.useShaderProgram();


		glm::mat4 view = myCamera.getViewMatrix();
		GLint viewLoc = glGetUniformLocation(snowShader.shaderProgram, "view");
		GLint viewLoc2 = glGetUniformLocation(fogShader.shaderProgram, "view");
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 snowflakeModel = glm::translate(glm::mat4(1.0f), snowflake.position);
		snowflakeModel = glm::scale(snowflakeModel, glm::vec3(0.0024f, 0.0124f, 0.0024f));
		GLint modelLoc = glGetUniformLocation(snowShader.shaderProgram, "model");
		GLint modelLoc2 = glGetUniformLocation(fogShader.shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(snowflakeModel));
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(snowflakeModel));

		// simple rendering (as a point or small quad)
		glDisable(GL_CULL_FACE);
		glBindVertexArray(snowflakeVAO);  // quad for snowflake
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glEnable(GL_CULL_FACE);
	}
}




void windowResizeCallback(GLFWwindow* window, int width, int height) {

	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
}




void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        wireframeMode = !wireframeMode;
    }

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        automatedTour = !automatedTour;
    }

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        pointMode = !pointMode;
    }


	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		// Set both the global and local weights to 0
		float localWeightVal = turnLights ? 0.0f : 0.4;
		float globalWeightVal = turnLights ? 0.0f : 0.6;

		myCustomShader.useShaderProgram();
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "localWeight"), localWeightVal);
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "globalWeight"), globalWeightVal);


		turnLights = !turnLights;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        stormMode = !stormMode;
        if (stormMode) {
            stormTimer = 0.0f;
            stormElapsed = 0.0f;
			NUM_SNOWFLAKES = 5000;
        }
    }

	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS) {
			pressedKeys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			pressedKeys[key] = false;
		}
	}
}

void playSound(const char* filename) {
    PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC);
}

void handleStormMode(float deltaTime) {
    if (stormMode) {
        stormTimer += deltaTime;
        lightningTimer += deltaTime;

        // check if its time for a lightning flash
        if (lightningTimer >= lightningInterval) {
            isThundering = true;
            ambientStrength[1] = 50.2f;
			playSound("C:\\Users\\gamer\\source\\repos\\Proiect GP\\Proiect GP\\thunder.wav"); // play the thunder sound
            lightningTimer = 0.0f;
            lightningInterval = lightningDistribution(generator); //set the next lightning interval
        }

        // reset the ambient strength
        if (isThundering) {
            stormElapsed += deltaTime;
            if (stormElapsed >= thunderDuration) {
                ambientStrength[1] = ambientStrength[2];
                stormElapsed = 0.0f;
                isThundering = false;
            }
        }

        // reset the storm timer after the storm duration
        if (stormTimer >= stormDuration) {
            ambientStrength[1] = ambientStrength[2];
            stormTimer = 0.0f;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	myCamera.processMouseMovement(xpos, ypos);
}

void processMovement()
{
	float cameraSpeed = static_cast<float>(2.5 * deltaTime);

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 0.05f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 0.05f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_SPACE]) {
		myCamera.move(gps::MOVE_UP, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_Z]) {
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);
	}
}

bool initOpenGLWindow() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	// for multisampling/antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Texturing", NULL, NULL);

	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

GLuint ReadTextureFromFile(const char* file_name) {

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);

	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}

	int width_in_bytes = x * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {

		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;

		for (int col = 0; col < width_in_bytes; col++) {

			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB, //GL_SRGB,//GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}



void cleanup() {

	if (quadVAO) {
		glDeleteVertexArrays(1, &quadVAO);
	}
	if (quadVAOScreen) {
		glDeleteVertexArrays(1, &quadVAOScreen);
	}
	if (snowflakeVAO) {
		glDeleteVertexArrays(1, &snowflakeVAO);
	}
	if (quadVBO) {
		glDeleteBuffers(1, &quadVBO);
	}
	if (quadVBOScreen) {
		glDeleteBuffers(1, &quadVBOScreen);
	}
	if (snowflakeVBO) {
		glDeleteBuffers(1, &snowflakeVBO);
	}
	if (snowflakeEBO) {
		glDeleteBuffers(1, &snowflakeEBO);
	}
	/*if (framebuffer) {
		glDeleteFramebuffers(1, &framebuffer);
	}
	if (rbo) {
		glDeleteRenderbuffers(1, &rbo);
	}*/

	glDeleteTextures(2, textureColorbuffer);

	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}


void initOpenGLState()
{
	glClearColor(0.4f, 0.6f, 0.9f, 1.0f); // light blue
	glViewport(0, 0, retina_width, retina_height);


	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_BLEND); // pentru transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void initObjects()
{
	myModel.LoadModel("objects/trees.obj", "textures/");
	myModel2.LoadModel("objects/landscape.obj", "textures/");
	myModel3.LoadModel("objects/house.obj", "textures/");
	myModel4.LoadModel("objects/snowman.obj", "textures/");
	lamp.LoadModel("objects/lamp.obj", "textures/");
	celestial.LoadModel("objects/celestial.obj", "textures/");
	podea.LoadModel("objects/podea.obj", "textures/");
	casa.LoadModel("objects/casa.obj", "textures/");


	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	casa.LoadModel("objects/podea.obj", "textures/");
	lirchuchici.LoadModel("objects/Sketchfab_2023_02_22_14_45_04.obj", "textures/");
	bottle1.LoadModel("objects/bottle1.obj", "textures/bottle1/");
	bottle2.LoadModel("objects/bottle2.obj", "textures/bottle2/");


	bottle3_1.LoadModel("objects/bottle3/part1.obj", "textures/bottle3/");
	bottle3_2.LoadModel("objects/bottle3/part2.obj", "textures/bottle3/");

	for (int i = 0; i < 60; i++) {
		// Construct the file path
		std::string path = "objects/anim/";
		std::string object = "frame" + std::to_string(i+1) + ".obj";
		path += object;

		std::string texturePath = "textures/anim/";

		// Load the model
		goofy[i].LoadModel(path, texturePath);
	}
}


void initUniforms()
{
	fogShader.useShaderProgram();

	glm::mat4 projection3 = glm::perspective(glm::radians(fov), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	GLint projLoc3 = glGetUniformLocation(fogShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection3));

	glm::mat4 view3 = myCamera.getViewMatrix();
	GLint viewLoc3 = glGetUniformLocation(fogShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view3));

	glm::mat4 model3 = glm::mat4(1.0f);
	GLint modelLoc3 = glGetUniformLocation(fogShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model3));



	//The snow shader
	snowShader.useShaderProgram();
	glm::mat4 projection2 = glm::perspective(glm::radians(fov), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	GLint projLoc2 = glGetUniformLocation(snowShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection2));

	glm::mat4 view2 = myCamera.getViewMatrix();
	GLint viewLoc2 = glGetUniformLocation(snowShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view2));

	glm::mat4 model2 = glm::mat4(1.0f);
	GLint modelLoc2 = glGetUniformLocation(snowShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
}




void initShaders() {
	skyShader.loadShader("shaders/shaderSky.vert", "shaders/shaderSky.frag");
	snowShader.loadShader("shaders/shaderSnow.vert", "shaders/shaderSnow.frag");
	fogShader.loadShader("shaders/shaderFog.vert", "shaders/shaderFog.frag");
	fogShaderObj.loadShader("shaders/shaderFogObj.vert", "shaders/shaderFogObj.frag");
	lightHouse.loadShader("shaders/shaderPPL.vert", "shaders/shaderPPL.frag");
	combined.loadShader("shaders/shaderCombined.vert", "shaders/shaderCombined.frag");


	screenFrame0.loadShader("shaders/sceneWithBlur.vert", "shaders/sceneWithBlur.frag");
	screenFrame1.loadShader("shaders/shaderPP1.vert", "shaders/shaderPP1.frag");
	shaderBlur.loadShader("shaders/blurKernel.vert", "shaders/blurKernel.frag");




	//shadows
	myCustomShader.loadShader("shaders/sm/shaderStart.vert", "shaders/sm/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/sm/lightCube.vert", "shaders/sm/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/sm/screenQuad.vert", "shaders/sm/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/sm/depthMap.vert", "shaders/sm/depthMap.frag");
	depthMapShader.useShaderProgram();




}


void initFrameBufferBase(GLuint& framebuffer, GLuint& rbo) {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create and attach depth/stencil renderbuffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2 * glWindowWidth, 2 * glWindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void attachColorChannel(GLuint framebuffer, GLuint& texture, GLenum attachment) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Create the texture for the color attachment
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2 * glWindowWidth, 2 * glWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setupFramebuffers() {
	// Initialize the base framebuffer and depth/stencil renderbuffer
	initFrameBufferBase(framebuffer[0], rbo[0]);

	initFrameBufferBase(framebuffer[1], rbo[1]);


	// Attach the first color channel
	attachColorChannel(framebuffer[0], textureColorbuffer[0], GL_COLOR_ATTACHMENT0);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	checkFramebufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Attach the second color channel
	attachColorChannel(framebuffer[1], textureColorbuffer[1], GL_COLOR_ATTACHMENT0);


	// Check framebuffer completeness
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	checkFramebufferStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


GLuint intermediateFramebuffer1;
GLuint intermediateTexture1;

GLuint intermediateFramebuffer2;
GLuint intermediateTexture2;


void initIntermediary()
{
	// Create the framebuffer
	glGenFramebuffers(1, &intermediateFramebuffer1);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer1);

	// Create the texture
	glGenTextures(1, &intermediateTexture1);
	glBindTexture(GL_TEXTURE_2D, intermediateTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2 * glWindowWidth, 2 * glWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTexture1, 0);

	// Check completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Intermediate framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void initIntermediary2()
{
	// Create the framebuffer
	glGenFramebuffers(1, &intermediateFramebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer2);

	// Create the texture
	glGenTextures(1, &intermediateTexture2);
	glBindTexture(GL_TEXTURE_2D, intermediateTexture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2 * glWindowWidth, 2 * glWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTexture2, 0);

	// Check completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Intermediate framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



//------------------------------------------------------------------------------------------ Turul de camera

glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float t) {
    return start * (1.0f - t) + end * t;
}


std::ifstream cameraPositionFile("camera_position.txt");
std::ifstream cameraTargetFile("camera_target.txt");


std::vector<glm::vec3> cameraPositions;
std::vector<glm::vec3> cameraTargets;


bool parseCameraLine(const std::string &line, const std::string &expectedLabel, glm::vec3 &outVec) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) {
        std::cerr << "Invalid line (no colon): " << line << std::endl;
        return false;
    }

    std::string labelPart = line.substr(0, colonPos);
    std::string valuesPart = line.substr(colonPos + 1);

    auto trim = [](std::string &s) {
        while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
        while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
    };
    trim(labelPart);
    trim(valuesPart);

    for (char &c : valuesPart) {
        if (c == ',') c = ' ';
    }

    if (labelPart != expectedLabel) {
        std::cerr << "Unexpected label: " << labelPart << ". Expected: " << expectedLabel << std::endl;
        return false;
    }

    std::istringstream iss(valuesPart);
    if (!(iss >> outVec.x >> outVec.y >> outVec.z)) {
        std::cerr << "Error parsing values from line: " << line << std::endl;
        return false;
    }

    return true;
}

bool readCameraData() {
    std::ifstream cameraPositionFile("camera_position.txt");
    std::ifstream cameraTargetFile("camera_target.txt");

    if (!cameraPositionFile.is_open()) {
        std::cerr << "Error opening camera_position.txt" << std::endl;
        return false;
    }

    if (!cameraTargetFile.is_open()) {
        std::cerr << "Error opening camera_target.txt" << std::endl;
        return false;
    }

    std::string linePos, lineTarget;
    cameraPositions.clear();
    cameraTargets.clear();

    while (true) {
        // Attempt to read one line from positions and one line from targets
        if (!std::getline(cameraPositionFile, linePos)) break;
        if (!std::getline(cameraTargetFile, lineTarget)) {
            std::cerr << "Mismatch in number of lines between position and target files." << std::endl;
            return false;
        }

        glm::vec3 cameraPos, cameraTar;
        if (!parseCameraLine(linePos, "Camera Position", cameraPos)) return false;
        if (!parseCameraLine(lineTarget, "Camera Target", cameraTar)) return false;

        cameraPositions.push_back(cameraPos);
        cameraTargets.push_back(cameraTar);
    }

    // If we reached EOF on positions, but not on targets, check
    if (std::getline(cameraTargetFile, lineTarget)) {
        std::cerr << "Mismatch in number of lines between position and target files." << std::endl;
        return false;
    }

    // If no data was read
    if (cameraPositions.empty() || cameraTargets.empty()) {
        std::cerr << "No camera data found." << std::endl;
        return false;
    }

    return true;
}


void updateCameraTour(float deltaTime) {
    static int currentIndex = 0;
    static float t = 0.0f;
    const float speed = 2.5f; // Adjust the speed of the tour

    t += speed * deltaTime;
    if (t > 1.0f) {
        t = 0.0f;
        currentIndex = (currentIndex + 1) % (cameraPositions.size() - 1);
    }

    glm::vec3 cameraPos = interpolate(cameraPositions[currentIndex], cameraPositions[currentIndex + 1], t);
    glm::vec3 cameraTarget = interpolate(cameraTargets[currentIndex], cameraTargets[currentIndex + 1], t);

    myCamera.setCameraPosition(cameraPos);
    myCamera.setCameraTarget(cameraTarget);
}

//------------------------------------------------------------------------------------------ Turul de camera



// TEST DE LA CODU CU SM
GLuint sceneFBO;
GLuint sceneTexture;

void initSceneFBO() {
	glGenFramebuffers(1, &sceneFBO);
	glGenTextures(1, &sceneTexture);

	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glWindowWidth, glWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glWindowWidth, glWindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void initUniformsShadows() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	GLuint normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	GLuint projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(30.0f, 15.0f, 5.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	GLuint lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);


	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//clamping similar to bloom!!! prevent artifacts wrapping around
	//border color is often set to white (1.0, 1.0, 1.0, 1.0) because it represents the maximum depth value, correct representation of BORDER OF CAMERA!!!!
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	//no need for color
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {

	glm::vec3 rotatedLightDir = glm::mat3(lightRotation) * lightDir; // ca sa se updateze la rotatiea luminii
	glm::mat4 lightView = glm::lookAt(rotatedLightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//the light is parallel!!!!!!!!!! thats why we use this!
	const GLfloat near_plane = 0.1f, far_plane = 40.0f;
	glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);

	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

	return lightSpaceTrMatrix;
}


int totalFrames = 60; 
int counter = 0;
int frameCount = 0;


void Animate(gps::Shader& shader, int frameCount) {
	if (frameCount % 20 == 0) {
		counter = (counter + 1) % totalFrames;
	}

	shader.useShaderProgram();

	goofy[counter].Draw(shader);
}



float oscillationAmplitude = 0.4f; // Amplitude of the oscillation
float oscillationFrequency = 0.02f;

glm::vec3 worldLightPos1 = glm::vec3(16.0f, 1.2f, 1.0f); // Define in world space
glm::vec3 worldLightPos2 = glm::vec3(15.0f, 1.2f, -1.0f); // Define in world space


void drawObjects(gps::Shader shader) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// Use the fog shader program for the skybox
	fogShader.useShaderProgram();

	// Set the view matrix
	glm::mat4 view = myCamera.getViewMatrix();
	GLint viewLoc = glGetUniformLocation(fogShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Set the projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	GLint projLoc = glGetUniformLocation(fogShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Render the skybox
	skyShader.useShaderProgram();
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	glDepthMask(GL_FALSE);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	glm::vec3 lightPosEyeObj1 = glm::vec3(view * glm::vec4(worldLightPos1, 1.0f));
	lightPosEye[1] = lightPosEyeObj1;

	glm::vec3 lightPosEyeObj2 = glm::vec3(view * glm::vec4(worldLightPos2, 1.0f));
	lightPosEye[2] = lightPosEyeObj2;


	cameraPosEye[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraPosEye[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 fogColor = glm::vec3(0.8f, 0.85f, 0.9f);
	float density = 0.02;
	float exponent = 1.5;


	if (pointMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    } else if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


	if (stormMode)
	{
		setUniformsCombined(shader,
			glm::vec3(0.2, 0.2, 0.2), // Color of the fog
			3, // Density of the fog
			0.23, // Exponent for the fog calculation
			2.2,// Global ambient light strength
			5.2, // Global specular light strength
			2); //shinyness


		celestial.Draw(shader);
		myModel.Draw(shader);
		myModel2.Draw(shader);
		myModel4.Draw(shader);
		lamp.Draw(shader);
		myModel3.Draw(shader);
		podea.Draw(shader);
		casa.Draw(shader);
		felinar.Draw(shader);
		bottle3_1.Draw(shader);

		GLint boolLocation = glGetUniformLocation(shader.shaderProgram, "isAlpha");
		bool value = true;
		glUniform1i(boolLocation, value);

		bottle1.Draw(shader);
		bottle2.Draw(shader);
		bottle3_2.Draw(shader);

		value = false;
		glUniform1i(boolLocation, value);
	}
	else
	{
		setUniformsCombined(shader,
			glm::vec3(0.9, 0.9, 0.9), // Color of the fog
			0, // Density of the fog
			0, // Exponent for the fog calculation
			2.2,// Global ambient light strength
			5.2, // Global specular light strength
			2); //shinyness



		celestial.Draw(shader);


		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			1.2, // Density of the fog
			0.18, // Exponent for the fog calculation
			0.001,// Global ambient light strength
			0.0, // Global specular light strength
			0.0001); //shinyness

		myModel.Draw(shader);
		myModel2.Draw(shader);
		myModel4.Draw(shader);

		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			0.8, // Density of the fog
			0.12, // Exponent for the fog calculation
			0.001,// Global ambient light strength
			0.0, // Global specular light strength
			0.0001); //shinyness

		lamp.Draw(shader);
		myModel3.Draw(shader);



		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			1.2, // Density of the fog
			0.18, // Exponent for the fog calculation
			0.01,// Global ambient light strength
			0.01, // Global specular light strength
			0.0001); //shinyness

		podea.Draw(shader);
		casa.Draw(shader);
		felinar.Draw(shader);
		bottle3_1.Draw(shader);

		//set and reset transparency
		GLint boolLocation = glGetUniformLocation(shader.shaderProgram, "isAlpha");
		bool value = true;
		glUniform1i(boolLocation, value);

		bottle1.Draw(shader);
		bottle2.Draw(shader);
		bottle3_2.Draw(shader);

		value = false;
		glUniform1i(boolLocation, value);

		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			0.1, // Density of the fog
			0.1, // Exponent for the fog calculation
			5.01,// Global ambient light strength
			15.01, // Global specular light strength
			5.0001); //shinyness

		float oscillation = oscillationAmplitude * sin(oscillationFrequency * lightAngle);
		model = lightRotation;
		model = glm::translate(model, glm::vec3(0.0f, oscillation, 0.0f)); // Apply oscillation on y-axis
		model = glm::translate(model, 1.5f * lightDir);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lirchuchici.Draw(shader);


		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			0.5, // Density of the fog
			0.15, // Exponent for the fog calculation
			0.001,// Global ambient light strength
			0.0, // Global specular light strength
			0.000001); //shinyness

		//---------------------------------------------------------------------- Ca sa translez monstruletul

		setUniformsCombined(shader,
			glm::vec3(0.5, 0.5, 0.5), // Color of the fog
			0.8, // Density of the fog
			0.12, // Exponent for the fog calculation
			0.001,// Global ambient light strength
			0.0, // Global specular light strength
			0.0001); //shinyness


		model = glm::translate(glm::mat4(1.0f), glm::vec3(26.5f, 3.3f, 11.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		view = myCamera.getViewMatrix();
		GLint viewLoc2 = glGetUniformLocation(shader.shaderProgram, "view");
		glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));


		projection = glm::perspective(glm::radians(fov), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
		GLint projLoc2 = glGetUniformLocation(shader.shaderProgram, "projection");
		glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));
		//---------------------------------------------------------------------- Ca sa translez monstruletul



		Animate(shader, frameCount);
		frameCount++;

		lightShader.useShaderProgram();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.0f * lightDir);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


		//reset fill type 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	
}
// TEST DE LA CODU CU SM




void FrameBufferRender()
{

	// Render the scene in the depth map
	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	// Set the viewport to the window size
	glViewport(0, 0, 2 * glWindowWidth, 2 * glWindowHeight);

	// ------------------------------------------------------------------------------FOR THE SCREEN RENDER!!!!!
	// Render the full scene to framebuffer[0]
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	myCustomShader.useShaderProgram();
	glm::mat4 view = myCamera.getViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	// Bind the shadow map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

	drawObjects(myCustomShader);
	renderSnowflakes();

	// Back to default framebuffer (not needed immediately, but ensures no accidental writes to framebuffer[0])
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ------------------------------------------------------------------------------FOR THE SCREEN RENDER!!!!!


	// ------------------------------------------------------------------------------FOR THE BLOOM RENDER!!!!!
	// Render the bright parts to framebuffer[1]
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	drawObjects(myCustomShader);
	renderSnowflakes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Back to default framebuffer
	// ------------------------------------------------------------------------------FOR THE BLOOM RENDER!!!!! intermediary 1


	// ------------------------------------------------------------------------------FOR THE FIRST INTERMEDIATE PASS!!!!!
	// Render the bright parts into intermediateFramebuffer1
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screenFrame1.useShaderProgram();
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer[1]); // Source: bright parts
	glBindVertexArray(quadVAOScreen);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Reset viewport in case intermediate framebuffer size differs
	glViewport(0, 0, 2 * glWindowWidth, 2 * glWindowHeight);
	// ------------------------------------------------------------------------------FOR THE FIRST INTERMEDIATE PASS!!!!!


	// ------------------------------------------------------------------------------FOR THE SECOND INTERMEDIATE PASS!!!!!
	// Apply blur to intermediateFramebuffer1 and render to intermediateFramebuffer2
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFramebuffer2);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderBlur.useShaderProgram();
	glUniform2f(glGetUniformLocation(shaderBlur.shaderProgram, "texelSize"), 1.0f / glWindowWidth, 1.0f / glWindowHeight);
	glBindTexture(GL_TEXTURE_2D, intermediateTexture1); // Source: intermediateTexture1
	glBindVertexArray(quadVAOScreen);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 2 * glWindowWidth, 2 * glWindowHeight);
	// ------------------------------------------------------------------------------FOR THE SECOND INTERMEDIATE PASS!!!!!


	// ------------------------------------------------------------------------------COMBINE BLOOM AND SCENE!!!!!
	glViewport(0, 0, retina_width, retina_height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenFrame0.useShaderProgram(); // Shader for combining textures
	glUniform1f(glGetUniformLocation(screenFrame0.shaderProgram, "bloomIntensity"), 2.0f); // Set bloom intensity
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer[0]); // Main scene texture
	glUniform1i(glGetUniformLocation(screenFrame0.shaderProgram, "sceneTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, intermediateTexture2); // Blurred bright parts
	glUniform1i(glGetUniformLocation(screenFrame0.shaderProgram, "bloomTexture"), 1);

	glBindVertexArray(quadVAOScreen);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// ------------------------------------------------------------------------------COMBINE BLOOM AND SCENE!!!!!
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov = myCamera.zoom(xoffset, yoffset, fov);
}



int main(int argc, const char* argv[]) {
	srand(10);
	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetScrollCallback(glWindow, scroll_callback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;


	initIntermediary();
	initIntermediary2();



	initQuadScreenVertices();
	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initUniformsShadows();
	initFBO();
	initSceneFBO();

	setupFramebuffers();


	initSnowFlakeData(); //VERY IMPORTANT
	initSnowflakes();

	if (!readCameraData()) {
        std::cerr << "Failed to read camera data." << std::endl;
        return 1;
    }



	while (!glfwWindowShouldClose(glWindow)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (automatedTour) {
            updateCameraTour(deltaTime);
        } else {
            processMovement();
        }

		handleStormMode(deltaTime);
		updateSnowflakes();
		FrameBufferRender();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();


	return 0;
}
