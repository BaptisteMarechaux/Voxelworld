#include "Voxel.h"



Voxel::Voxel(int indicesSize)
{
	transform = VTransform();
	//program = LoadShaders("..\\shaders\\simple.vs", "..\\shaders\\simple.fs");
	//mvp_location = glGetUniformLocation(program, "MVP");
	//position_location = glGetAttribLocation(program, "position");
	//color_location = glGetUniformLocation(program, "vertexColor");

	points = {
		glm::vec3(-0.5f,-0.5f,-0.5f), // triangle 1 : begin
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f), // triangle 1 : end
		glm::vec3(0.5f, 0.5f,-0.5f), // triangle 2 : begin
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f), // triangle 2 : end
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f)
	};

	ComputeIndices(indicesSize);

	std::vector<GLuint> checkedIndices = std::vector<GLuint>();
	for (unsigned int i = 0; i < points.size(); i++)
	{
		glm::vec3 pointNormal = glm::vec3();
		for (int j = 0; j < indices.size(); j+=3)
		{
			if (indices[j] == i || indices[j + 1] == i || indices[j + 2] == i)
			{
				glm::vec3 edge1 = points[indices[j + 1]] - points[indices[j]];
				glm::vec3 edge2 = points[indices[j + 2]] - points[indices[j + 1]];
				glm::vec3 triangleNormal = glm::cross(edge1, edge2);
				pointNormal += triangleNormal;
			}
			
		}
		normals.push_back(glm::normalize(pointNormal));
	}

	std::cout << "Added new Voxel" << std::endl;
}


Voxel::~Voxel()
{
}

void Voxel::SetPosition(glm::vec3 pos)
{
	transform.position = pos;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		glm::mat4 myMatrix = glm::translate(transform.position);
		glm::vec4 myVec(points[i].x, points[i].y, points[i].z, 1);
		glm::vec4 transformedVector = myMatrix * myVec;
		points[i] = glm::vec3(transformedVector.x, transformedVector.y, transformedVector.z);
	}
		
}

void Voxel::ComputeIndices(int indicesSize)
{
	indices = std::vector<GLuint>();
	std::vector<glm::vec3> out = std::vector<glm::vec3>();
	bool found = false;
	GLuint indexFound;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		for (unsigned int j = 0; j < out.size(); j++)
		{
			if (points[i] == out[j])
			{
				found = true;
				indexFound = indices[j];
				break;
			}
		}
		if (found)
		{
			indices.push_back(indexFound + indicesSize);
		}
		else
		{
			out.push_back(points[i]);
			indices.push_back(out.size()-1 + indicesSize);
		}
		found = false;
	}
	points.clear();
	points = out;

}

std::vector<glm::vec3> Voxel::getPoints()
{
	return points;
}

std::vector<GLuint> Voxel::getIndices()
{
	/*
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
	*/
	return indices;
}

std::vector<glm::vec3> Voxel::getNormals()
{
	return normals;
}

std::vector<GLuint> Voxel::getIndices(int indicesSize)
{
	std::vector<GLuint> indices = std::vector<GLuint>();
	std::vector<glm::vec3> out = std::vector<glm::vec3>();
	bool found = false;
	GLuint indexFound;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		for (unsigned int j = 0; j < out.size(); j++)
		{
			if (points[i] == out[j])
			{
				found = true;
				indexFound = indices[j];
				break;
			}
		}
		if (found)
		{
			indices.push_back(indexFound+indicesSize);
		}
		else
		{
			out.push_back(points[i]);
			indices.push_back(i+indicesSize);
		}
		found = false;
	}

	return indices;
}
