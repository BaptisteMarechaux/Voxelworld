#include "Voxel.h"



Voxel::Voxel()
{
	transform = VTransform();
	//program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");
	//mvp_location = glGetUniformLocation(program, "MVP");
	//position_location = glGetAttribLocation(program, "position");
	//color_location = glGetUniformLocation(program, "vertexColor");

	points = {
		glm::vec3(-1.0f,-1.0f,-1.0f), // triangle 1 : begin
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
		glm::vec3(1.0f, 1.0f,-1.0f), // triangle 2 : begin
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f), // triangle 2 : end
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f,-1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f,-1.0f, 1.0f)
	};
}


Voxel::~Voxel()
{
}

void Voxel::SetPosition(glm::vec3 pos)
{
	transform.position = pos;
}

std::vector<glm::vec3> Voxel::getPoints()
{
	return points;
}

std::vector<GLuint> Voxel::getIndices()
{
	std::vector<GLuint> indices = std::vector<GLuint>();
	std::vector<glm::vec3> out = std::vector<glm::vec3>();
	bool found = false;
	GLuint indexFound;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		for (unsigned int j = 0; j < out.size(); j ++)
		{
			if(points[i] == out[j])
			{
				found = true;
				indexFound = indices[j];
				break;
			}
		}
		if (found)
		{
			indices.push_back(i);
		}
		else
		{
			out.push_back(points[i]);
			indices.push_back(i);
		}
		found = false;
	}

	return indices;
}
