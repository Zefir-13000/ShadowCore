#include "Transform.h"

void Transform::Update() {
	Transform::model = Transform::T * Transform::S * Transform::R;
}

void Transform::Translate(const glm::vec3& pos) {
	Transform::T = glm::translate(pos);
	Transform::Update();
}
void Transform::Rotate(const glm::vec3& rot) {
	glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));

	Transform::R = rotZ * rotY * rotX;
	Transform::Update();
}
void Transform::Scale(const glm::vec3& scale) {
	Transform::S = glm::scale(scale);
	Transform::Update();
}