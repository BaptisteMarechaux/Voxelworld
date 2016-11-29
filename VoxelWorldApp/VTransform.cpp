#include "VTransform.h"



VTransform::VTransform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(0.0f);
}

VTransform::VTransform(glm::vec3, glm::vec3, glm::vec3)
{

}


VTransform::~VTransform()
{
}
