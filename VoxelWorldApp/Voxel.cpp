#include "Voxel.h"



Voxel::Voxel()
{
	transform = VTransform();

	points = {
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f)

	};

	indices = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		2, 3, 7,
		2, 7, 6,
		3, 0, 4,
		3, 4, 7,
		4, 5, 6,
		4, 6, 7
	};

	//ComputeIndices(indicesSize);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		glm::vec3 pointNormal = glm::vec3();
		for (int j = 0; j < indices.size(); j+=3)
		{
			if (indices[j] == i || indices[j + 1] == i || indices[j + 2] == i)
			{
				glm::vec3 edge1 = points[indices[j + 1]] - points[indices[j]];
				glm::vec3 edge2 = points[indices[j + 2]] - points[indices[j]];
				glm::vec3 triangleNormal = glm::cross(edge1, edge2);
				pointNormal += triangleNormal;
			}
			
		}
		normals.push_back(glm::normalize(pointNormal));
	}
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
	return indices;
}

std::vector<glm::vec3> Voxel::getNormals()
{
	return normals;
}

std::vector<GLuint> Voxel::getIndices(int indicesSize)
{
	std::vector<GLuint> returnedIndices = std::vector<GLuint>();
	for (unsigned int i = 0; i < indices.size(); i++)
		returnedIndices.push_back(indices[i] + indicesSize);

	return returnedIndices;
}
