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

class Scene
{
private:
	std::list<Voxel*> voxelItems;
	glm::mat4 model, proj, mvp, view;
	GLuint MatrixID, VertexArrayID; //MVP Matrix
	GLfloat defaultFragmentColor[4] = { 1, 0, 0, 1 };
	GLuint vertexBufferPoints;
	GLuint voxelElementBuffer; //Element Array Buffer Pour les indices de faces de Voxels
	GLuint uvbuffer, normalbuffer;
	std::vector<glm::vec3> g_vertex_buffer_data;
	std::vector<GLuint> indices;

	GLuint program;
	GLuint position_location, color_location, mvp_location, light_location;

	Input input;

	//Camera management
	glm::vec3 camPosition = glm::vec3(0, 0, 5);
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 60.0f;

	float camSpeed = 3.0f;
	float mouseSpeed = 0.005f;

public:
	Scene();
	~Scene();

	void Initialize();
	void Render();
	void Update();
	void UpdateBuffer(GLint* buffer);
	void AddVoxelAtPosition(glm::vec3 pos);
	void TranslateCamera(glm::vec3 v);
	int getVertexCount();
};

