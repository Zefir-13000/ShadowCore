#pragma once
#include "ssi.h"

namespace SC {

	class Object;
	class Component {
	public:
		std::shared_ptr<Object> parent = nullptr;

		ComponentType component_type = COMPONENT;
		Component(std::shared_ptr<Object> _parent) {
			this->parent = _parent;
		}

		virtual void Render() {};
		virtual void Update() {};
		virtual ~Component() {}
	};

};