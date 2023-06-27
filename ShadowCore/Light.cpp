#include "Light.h"

PointLight::PointLight(glm::vec3 _light_color, float _intensity = 1.0f) {
	PointLight::lightType = POINT_LIGHT;

	PointLight::light_color = _light_color;
	PointLight::intensity = _intensity;
}

PointLight::PointLight(float _intensity = 1.0f) {
	PointLight::lightType = POINT_LIGHT;

	PointLight::intensity = _intensity;
}
