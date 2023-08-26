#pragma once
#include "ssi.h"
#include "Component/Component.h"

constexpr int MAXLEN_OBJECT_NAME = 255;

namespace SC {

	class Engine;
	class Object : public std::enable_shared_from_this<Object>
	{
	protected:
		std::shared_ptr<Engine> enginePtr = nullptr;
		static uint32_t Last_id;
		uint32_t id;
	public:
		UseLessType u_type = USELESS;
		std::vector<std::shared_ptr<Component>> components;

		std::string name;
		ObjectType type = OBJECT;
		Object();
		Object(_In_ std::string name);

		void SetName(std::string _name) { this->name = _name; }

		uint32_t getId() const { return id; }

		template <class T>
		std::shared_ptr<T> AddComponent() {
			std::shared_ptr<Component> component = nullptr;
			component = std::make_shared<T>(shared_from_this());
			this->components.push_back(component);
			return std::dynamic_pointer_cast<T>(component);
		}

		template <class T> 
		std::shared_ptr<T> GetComponent() {
			for (std::shared_ptr<Component> component : components) {
				if (std::shared_ptr<T> d_comp = std::dynamic_pointer_cast<T>(component)) {
					return d_comp;
				}
			}
			return nullptr;
		}

		void RemoveComponent(std::shared_ptr<Component> _component) {
			this->components.erase(std::remove(this->components.begin(), this->components.end(), _component), this->components.end());
		}

		bool IsRenderAble() {
			return this->type == RENDER_OBJECT || this->type == MESH;
		}

		virtual void Update() = 0;
		void Destroy();

		virtual ~Object() {}
	};

};