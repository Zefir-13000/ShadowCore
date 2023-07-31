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
	public:
		std::vector<std::shared_ptr<Component>> components;

		std::string name;
		ObjectType type = OBJECT;
		Object();
		Object(_In_ std::string name) {
			if (name.size() > 0 && name.size() < MAXLEN_OBJECT_NAME)
				Object::name = name;
			else
				Object::name = std::string("Object");
		}

		void AddComponent(std::shared_ptr<Component> _component) {
			this->components.push_back(_component);
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

		void Destroy();

		virtual ~Object() {}
	};

};