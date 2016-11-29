#pragma once
#include <vector>
#include "glm.hpp"
class VTransform
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	VTransform();
	VTransform(glm::vec3, glm::vec3, glm::vec3);
	~VTransform();
};

