#include "Light.h"
#include "Core/Core.h"

using namespace SC;

PointLight::PointLight(glm::vec3 _light_color, float _intensity = 1.0f){
	PointLight::name = "PointLight";
	PointLight::lightType = POINT_LIGHT;

	PointLight::light_color = _light_color;
	PointLight::intensity = _intensity;
}

PointLight::PointLight(float _intensity = 1.0f) {
	PointLight::name = "PointLight";
	PointLight::lightType = POINT_LIGHT;

	PointLight::intensity = _intensity;
}

void PointLight::Update() {
	// omnidirectional shadow
}

DirectionalLight::DirectionalLight(glm::vec3 _direction, float _intensity = 1.0f) {
	DirectionalLight::name = "DirectionalLight";
	DirectionalLight::lightType = DIRECTIONAL_LIGHT;

	DirectionalLight::intensity = _intensity;
}

void DirectionalLight::InitShadows() {
	if (DirectionalLight::cascade_shadow_map != nullptr) 
		return;

	std::shared_ptr<CascadeShadowMap> csm = std::make_shared<CascadeShadowMap>(std::dynamic_pointer_cast<DirectionalLight>(shared_from_this()), SHADOW_2k);
	DirectionalLight::cascade_shadow_map = csm;
}

void DirectionalLight::Update() {
	if (enginePtr->engine_active_shader == nullptr)
		return;

	enginePtr->engine_active_shader->setValue("light.direction", DirectionalLight::direction);
	enginePtr->engine_active_shader->setValue("light.AmbientStrength", glm::vec3(0.2f));
	enginePtr->engine_active_shader->setValue("light.DiffuseStrength", glm::vec3(0.5f));
	enginePtr->engine_active_shader->setValue("light.SpecularStrength", glm::vec3(1.0f));

	if (DirectionalLight::cascade_shadow_map != nullptr)
		DirectionalLight::cascade_shadow_map->Update();
}
