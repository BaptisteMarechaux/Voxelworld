#pragma once
#include <vector>
#include "Voxel.h"
#include "glm.hpp"

class Chunk
{
private:
	bool visible;
	int size;

public:
	std::vector<std::vector<std::vector<Voxel>>> voxels;
	Chunk(int size, glm::vec3 pos);
	~Chunk();

	///<summary>
	///Set the visibility of a chunk
	///True or False
	///</summary>
	void setVisiblity(bool visibility);
	bool getVisibility() { return visible; }
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> getNormals();
	void deleteRandomVoxels(float probability);

};

