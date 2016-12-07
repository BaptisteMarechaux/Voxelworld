#pragma once
#include <vector>
#include "glm.hpp"
#include "VTransform.h"
#include <gl3w\GL\gl3w.h>
#include <glfw\include\GLFW\glfw3.h>
#include "ShaderManager.h"
#include <glm\glm\gtx\transform.hpp>

class Voxel
{
private:
	std::vector<glm::vec3> points; //chaque int devra laisser place � 2 00 � la fin pour la partie flotante
	//Exemple 200 �quivaut donc � 2.00, 3005 �quivaut � 30.05
public:
	GLint program;
	GLuint position_location, color_location, mvp_location;
	VTransform transform;
	Voxel();
	Voxel(GLint _program) : program(_program) {}
	Voxel(GLint _program, glm::vec3 position) : program(_program)
	{
		transform = VTransform(position, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	}
	~Voxel();

	void SetPosition(glm::vec3 pos);
	std::vector<glm::vec3> getPoints();
	std::vector<GLuint> getIndices();
};

