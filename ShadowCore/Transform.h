#pragma once
#include "ssi.h"

class Transform {
private:
	glm::mat4 T = glm::mat4(1.), R = glm::mat4(1.), S = glm::mat4(1.);
	void Update();
public:
	Transform() {};
	~Transform() {};

	void Translate(const glm::vec3& pos);
	void Rotate(const glm::vec3& rot);
	void Scale(const glm::vec3& scale);

	glm::mat4 model = glm::mat4(1.);

	glm::vec3 forward{}, up{}, right{};

	glm::vec3 position{};
	glm::vec3 rotation{};
	glm::vec3 scale = glm::vec3(1.);
};