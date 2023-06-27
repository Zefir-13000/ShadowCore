#include "Transform.h"

void Transform::Update() {
	Transform::model = Transform::T * Transform::R * Transform::S;

	Transform::forward = glm::vec3(cos(Transform::rotation.x) * sin(Transform::rotation.y),sin(Transform::rotation.x),cos(Transform::rotation.x) * cos(Transform::rotation.y));
	Transform::right = glm::vec3(sin(Transform::rotation.y - 3.14f / 2.0f),0,cos(Transform::rotation.y - 3.14f / 2.0f));
	Transform::up = glm::cross(right, forward);
}

void Transform::Translate(const glm::vec3& pos) {
	Transform::position = pos;
	Transform::T = glm::translate(pos);
	Transform::Update();
}
void Transform::Rotate(const glm::vec3& rot) {
	Transform::rotation = rot;
	glm::mat4 rotX = glm::rotate(glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

	Transform::R = rotZ * rotY * rotX;
	Transform::Update();
}
void Transform::Scale(const glm::vec3& scale) {
	Transform::scale = scale;
	Transform::S = glm::scale(scale);
	Transform::Update();
}