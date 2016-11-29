#pragma once
#include <vector>
#include "glm.hpp"
#include "VTransform.h"
class Voxel
{
private:
	std::vector<int> points; //chaque int devra laisser place à 2 00 à la fin pour la partie flotante
	//Exemple 200 équivaut donc à 2.00, 3005 équivaut à 30.05
public:
	VTransform transform;
	Voxel();
	~Voxel();
};

