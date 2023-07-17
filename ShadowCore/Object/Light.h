#pragma once
#include "ssi.h"
#include "Object/Object.h"
#include "Common/Transform.h"

namespace SC {

	class Light : public Object
	{
	private:
		size_t lightID = 0;

	public:
		Light() {};
		~Light() {};

		LightType lightType = POINT_LIGHT;

		float intensity = 1.0f;
		glm::vec3 light_color = glm::vec4(1.0f);

		Transform transform;
	};

	class PointLight : public Light {
	public:
		PointLight(glm::vec3 _light_color, float _intensity);
		PointLight(float _intensity);
	};

};