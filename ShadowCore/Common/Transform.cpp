#include "Transform.h"
#include "Object/RenderObject.h"

using namespace SC;

void Transform::Update() {
	Transform::T = glm::translate(Transform::position);
	glm::mat4 rotX = glm::rotate(glm::radians(Transform::rotation.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(glm::radians(Transform::rotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(glm::radians(Transform::rotation.z), glm::vec3(0.0, 0.0, 1.0));
	Transform::R = rotZ * rotY * rotX;
	Transform::S = glm::scale(Transform::scale);

	Transform::model = Transform::T * Transform::R * Transform::S;

	Transform::forward = glm::normalize(glm::vec3(Transform::R * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	Transform::right = glm::normalize(glm::vec3(Transform::R * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	Transform::up = glm::normalize(glm::cross(Transform::right, Transform::forward));

	Transform::UpdateChildren(Transform::model);
}

void Transform::PrivateUpdate() {
	Transform::model = Transform::T * Transform::R * Transform::S;
	Transform::local_model = Transform::LT * Transform::LR * Transform::LS;
	
	Transform::forward = glm::normalize(glm::vec3(Transform::R * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	Transform::right = glm::normalize(glm::vec3(Transform::R * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	Transform::up = glm::normalize(glm::cross(Transform::right, Transform::forward));
}

void Transform::UpdateChildren(glm::mat4 RootM) {
	glm::mat4 nexttranslate = RootM * Transform::local_model;

	Transform::Translate(glm::vec3(nexttranslate[3]));
	Transform::Rotate(glm::eulerAngles(glm::quat(nexttranslate)));
	Transform::Scale(glm::vec3(glm::length(glm::vec3(nexttranslate[0])), glm::length(glm::vec3(nexttranslate[1])), glm::length(glm::vec3(nexttranslate[2]))));

	for (std::shared_ptr<Object> child : Transform::children) {
		if (child && child->IsRenderAble())
			std::dynamic_pointer_cast<RenderObject>(child)->transform->UpdateChildren(nexttranslate);
	}
}

void Transform::Translate(const glm::vec3& pos) {
	Transform::position = pos;
	Transform::T = glm::translate(pos);
	Transform::PrivateUpdate();
}

void Transform::LocalTranslate(const glm::vec3& pos) {
	Transform::local_position = pos;
	Transform::LT = glm::translate(pos);
	Transform::PrivateUpdate();
}

void Transform::Rotate(const glm::vec3& rot) {
	Transform::rotation = rot;
	glm::mat4 rotX = glm::rotate(glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

	Transform::R = rotZ * rotY * rotX;
	Transform::PrivateUpdate();
}

void Transform::LocalRotate(const glm::vec3& rot) {
	Transform::local_rotation = rot;
	glm::mat4 rotX = glm::rotate(glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotY = glm::rotate(glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZ = glm::rotate(glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

	Transform::LR = rotZ * rotY * rotX;
	Transform::PrivateUpdate();
}

void Transform::Scale(const glm::vec3& scale) {
	Transform::scale = scale;
	Transform::S = glm::scale(scale);
	Transform::PrivateUpdate();
}

void Transform::LocalScale(const glm::vec3& scale) {
	Transform::local_scale = scale;
	Transform::LS = glm::scale(scale);
	Transform::PrivateUpdate();
}

void Transform::AddChild(std::shared_ptr<Object> _child) {
	Transform::children.push_back(_child);
}