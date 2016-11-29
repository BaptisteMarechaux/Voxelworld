#pragma once
#include <vector>
#include "glm.hpp"
#include "VTransform.h"
class Voxel
{
private:
	std::vector<int> points; //chaque int devra laisser place � 2 00 � la fin pour la partie flotante
	//Exemple 200 �quivaut donc � 2.00, 3005 �quivaut � 30.05
public:
	VTransform transform;
	Voxel();
	~Voxel();
};

