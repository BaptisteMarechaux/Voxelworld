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
	std::vector<Voxel*> voxelItems;
	glm::mat4 model, proj, mvp, view;
	GLuint MatrixID, VertexArrayID, LightID, ModelMatrixID, ViewMatrixID, deltaTimeID;
	GLfloat defaultFragmentColor[4] = { 1, 0, 0, 1 };
	GLuint vertexBufferPoints;
	GLuint voxelElementBuffer; //Element Array Buffer Pour les indices de faces de Voxels
	GLuint uvbuffer, normalbuffer;
	std::vector<glm::vec3> g_vertex_buffer_data, normals, positions, vertices;
	std::vector<GLuint> indices;

	GLuint voxelVertexArrayID;

	GLuint program;
	GLuint position_location, color_location, mvp_location, light_location;

	float lastTime;
	float currentTime;
	float deltaTime;

	float lightPos[3] = { 1, 1, 0 };

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

public:
	Scene();
	~Scene();

	void Initialize();
	void Render();
	void UpdateBuffers();
	void AddVoxelAtPosition(glm::vec3 pos);
	void AddChunkAtPosition(glm::vec3 pos);
	void TranslateCamera(glm::vec3 v);
	int getVertexCount();
	void computeMatrixes(int winWidth, int winHeight, double xPos, double yPos);
	void zoomFoV(float);

	void Destroy();
};

