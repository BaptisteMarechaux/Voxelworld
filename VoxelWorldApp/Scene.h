#pragma once
#include "imgui\imgui.h"
#include <stdio.h>
#include <vector>
#include <list>
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Input.h"
#include "Voxel.h"
#include "Chunk.h"

class Scene
{
private:
	//Matrix
	glm::mat4 model, proj, mvp, view;
	//General Elements
	GLfloat defaultFragmentColor[4] = { 0.6f, 0, 0.4f, 1 };

	//Buffers
	GLuint vertexBufferPoints;
	GLuint uvbuffer, normalbuffer, occlusioncolorbuffer;

	std::vector<glm::vec3> normals, positions, vertices;
	std::vector<GLuint> indices;

	//Shader References
	GLuint program, ssaoProgram;
	GLuint position_location, color_location, mvp_location, light_location;
	GLuint MatrixID, VertexArrayID, LightID, ModelMatrixID, ViewMatrixID, deltaTimeID;

	//SSAO Locations
	GLuint ssao_posTextureUnitLocation, ssao_sampleRadLocation, ssao_projMatrixLocation, ssao_kernelLocation;

	//VAO
	GLuint voxelVertexArrayID;


	float lastTime;
	float currentTime;
	float deltaTime;

	float lightPos[3] = { 3, 3, 0 };

	Input input;

	std::vector<glm::vec3> computedVertices, computedNormals;
	std::vector<GLuint> computedIndices;

	//Chunks
	std::vector<Chunk> chunks;

	//Camera management
	glm::vec3 camPosition = glm::vec3(4, 3, 10);
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = glm::radians(45.0f);
	float FoV;
	glm::vec3 direction, up, right;

	float camSpeed = 0.2f;
	float mouseSpeed = 0.001f;
	glm::vec3 cameraPosition;

	//SSAO
	int kernelSize = 10;
	std::vector<glm::vec3> kernel;

public:
	Scene();
	~Scene();

	void Initialize();
	void Render();
	void UpdateBuffers();
	void AddVoxelAtPosition(glm::vec3 pos);
	void AddChunkAtPosition(glm::vec3 pos, int chunkSize);
	void AddSpherizedChunkAtPosition(glm::vec3 pos, int chunkSize);
	glm::vec3 getCameraPosition();
	void TranslateCamera(glm::vec3 v);
	int getVertexCount();
	void computeMatrixes(int winWidth, int winHeight, double xPos, double yPos);
	void zoomFoV(float);

	void AutoRotateCamera(float speed, float distance=500);

	//Render Passes
	void GeometryPass();
	void SSAOPass();
	void BlurPass();

	//SSAO
	void GenerateSampleKernel();

	float RandomFloat(float a, float b);
	void resetScene();
	void Destroy();
};

