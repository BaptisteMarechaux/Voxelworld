#pragma once
#include "imgui\imgui.h"
#include <stdio.h>
#include <vector>
#include <list>
#include "glm.hpp"
#include "Voxel.h"

class Scene
{
private:
	Voxel voxelItems[];

public:
	Scene();
	~Scene();

	void Initialize();
	void Render();
	void AddVoxelAtPosition(glm::vec3 pos);
};

