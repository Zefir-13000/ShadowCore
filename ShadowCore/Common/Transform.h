#pragma once
#include "ssi.h"

namespace SC {

	class Object;
	class RenderObject;
	class Transform {
	private:
		glm::mat4 T = glm::mat4(1.), R = glm::mat4(1.), S = glm::mat4(1.);
		glm::mat4 LT = glm::mat4(1.), LR = glm::mat4(1.), LS = glm::mat4(1.);

		glm::mat4 model = glm::mat4(1.);
		glm::mat4 local_model = glm::mat4(1.f);

		std::vector<std::shared_ptr<Object>> children = {};
		std::shared_ptr<Object> parent = nullptr;

		void PrivateUpdate();
	public:
		Transform() {};
		~Transform() {};

		void Update();
		void UpdateChildren(glm::mat4 RootM = glm::mat4(1.f));
		void Translate(const glm::vec3& pos);
		void LocalTranslate(const glm::vec3& pos);
		void Rotate(const glm::vec3& rot);
		void LocalRotate(const glm::vec3& rot);
		void Scale(const glm::vec3& scale);
		void LocalScale(const glm::vec3& scale);

		glm::mat4 GetMatrix() const { return Transform::model; }
		glm::mat4 GetLocalMatrix() const { return Transform::local_model; }

		void AddChild(std::shared_ptr<Object> _child);
		const std::vector<std::shared_ptr<Object>>& GetChildren() const {
			return Transform::children;
		}

		void SetParent(std::shared_ptr<Object> _parent) {
			Transform::parent = _parent;
		}
		std::shared_ptr<Object> GetParent() {
			return Transform::parent;
		}


		glm::vec3 forward{}, up{}, right{};

		glm::vec3 position{};
		glm::vec3 rotation{};
		glm::vec3 scale = glm::vec3(1.);

		glm::vec3 local_position{};
		glm::vec3 local_rotation{};
		glm::vec3 local_scale = glm::vec3(1.);

		RenderObject* this_obj = nullptr;
	};

};