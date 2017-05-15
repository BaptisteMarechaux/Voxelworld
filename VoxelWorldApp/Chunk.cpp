#include "Chunk.h"


Chunk::Chunk(int size, glm::vec3 pos)
{
	this->size = size;
	voxels = std::vector<std::vector<std::vector<Voxel>>>();
	for (int x = 0; x < size; x++)
	{
		voxels.push_back(std::vector<std::vector<Voxel>>());
		for (int y = 0; y < size; y++)
		{
			voxels[x].push_back(std::vector<Voxel>());
			for (int z = 0; z < size; z++)
			{
				voxels[x][y].push_back(Voxel());
				voxels[x][y][z].SetPosition(glm::vec3(x + pos.x, y + pos.y, z + pos.z));
				positions.push_back(glm::vec3(x + pos.x, y + pos.y, z + pos.z));
			}
		}
	}
}


Chunk::~Chunk()
{
}

void Chunk::setVisiblity(bool visibility)
{
	visible = false;
}

std::vector<glm::vec3> Chunk::getVertices()
{
	std::vector<glm::vec3> returnedVertices;
	for (int x = 0; x < size; x++)
	{		
		for (int y = 0; y < size; y++)
		{	
			for (int z = 0; z < size; z++)
			{
				if (voxels[x][y][z].visible)
				{
					std::vector<glm::vec3> v = voxels[x][y][z].getPoints();
					returnedVertices.insert(returnedVertices.end(), v.begin(), v.end()); // voxels[x][y][z]
				}
				
			}
		}
	}
	return returnedVertices;
}

std::vector<glm::vec3> Chunk::getNormals()
{
	std::vector<glm::vec3> returnedNormals;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				if (voxels[x][y][z].visible)
				{
					std::vector<glm::vec3> v = voxels[x][y][z].getNormals();
					returnedNormals.insert(returnedNormals.end(), v.begin(), v.end()); // voxels[x][y][z]
				}

			}
		}
	}
	return returnedNormals;
}

std::vector<glm::vec3> Chunk::getPositions()
{
	return positions;
}

void Chunk::deleteRandomVoxels(float probability)
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				if (rand() > 0.5f)
				{
					voxels[x][y][z].visible=false;
				}
				
			}
		}
	}
}

void Chunk::Spherize()
{
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				if (sqrt((float)(x - size / 2)*(x - size / 2) + (y - size / 2)*(y - size / 2) + (z - size / 2)*(z - size / 2)) <= size / 2)
				{
					voxels[x][y][z].visible = true;
				}
				else
				{
					voxels[x][y][z].visible = false;
				}

			}
		}
	}
}
