#pragma once
#include "ssi.h"
#include "Object/RenderObject.h"
#include "Common/Transform.h"
#include "Object/ShadowMap.h"

namespace SC {

	class Light : public RenderObject
	{
	private:
		uint32_t lightID = 0;

	protected:
		Light() : RenderObject("Light", std::make_shared<GeometryData>(), NEEDED) {};
		~Light() {};

		LightType lightType = NOLIGHT;

		float intensity = 1.0f;
		glm::vec3 light_color = glm::vec4(1.0f);
	};

	class DirectionalLight : public Light {
	public:
		DirectionalLight(glm::vec3 _direction, float _intensity);

		void InitShadows();
		void Update() override;

		glm::vec3 direction = glm::vec3(1.0f);
		std::shared_ptr<CascadeShadowMap> cascade_shadow_map = nullptr;
	};

	class PointLight : public Light {
	public:
		PointLight(glm::vec3 _light_color, float _intensity);
		PointLight(float _intensity);

		void Update() override;
	};

};