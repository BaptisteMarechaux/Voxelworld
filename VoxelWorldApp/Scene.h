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

#include "Voxel.h"

class Scene
{
private:
	std::list<Voxel*> voxelItems;
	glm::mat4 model, proj, mvp, view;
	GLuint MatrixID; //MVP Matrix
	GLfloat defaultFragmentColor[4] = { 1, 0, 0, 1 };
	GLuint vertexBufferPoints;
	std::vector<GLfloat> g_vertex_buffer_data;

public:
	Scene();
	~Scene();

	void Initialize();
	void Render();
	void Update();
	void AddVoxelAtPosition(glm::vec3 pos, GLint program);
	void TranslateCamera(glm::vec3 v);
};

