#pragma once
#include "glm.hpp"
class Input
{
private:
	int v;
public:
	Input();
	~Input();
	void computeMatrixes(glm::mat4* projection, glm::mat4* view, glm::mat4* model);
};

