#pragma once
#include "ssi.h"

namespace SC {

	class Component {
	public:
		ComponentType component_type = COMPONENT;
		Component() {}

		virtual void Render() {};
		virtual ~Component() {}
	};

};