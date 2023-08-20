#pragma once
#include "ssi.h"
#include "Object/RenderObject.h"
#include "Common/Transform.h"

namespace SC {

	class Light : public RenderObject
	{
	private:
		size_t lightID = 0;

	public:
		Light() : RenderObject("Light", std::make_shared<GeometryData>(), NEEDED) {};
		~Light() {};

		LightType lightType = NOLIGHT;

		float intensity = 1.0f;
		glm::vec3 light_color = glm::vec4(1.0f);
	};

	class PointLight : public Light {
	public:
		PointLight(glm::vec3 _light_color, float _intensity);
		PointLight(float _intensity);
	};

};