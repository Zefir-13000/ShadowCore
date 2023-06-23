#pragma once
#include "ssi.h"

class Transform {
public:
	Transform() {};
	~Transform() {};

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};