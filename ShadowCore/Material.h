#pragma once
#include "ssi.h"
#include "Texture.h"

class Material
{
public:
	MaterialType type = MATERIAL;

	virtual MaterialType GetType();

	Material() {};
	~Material() {};
};

class SMaterial : public Material {
public:
	glm::vec3 Ambient{ 0.1f };
	std::shared_ptr<Texture> ambient_texture = nullptr;
	glm::vec3 Diffuse{ 0.87, 0.5, 0.27 };
	std::shared_ptr<Texture> diffuse_texture = nullptr;
	glm::vec3 Specular{0.5f};
	std::shared_ptr<Texture> specular_texture = nullptr;
	glm::vec3 Emission{0.0f};
	std::shared_ptr<Texture> emission_texture = nullptr;
	float Shininess = 32.f;

	SMaterial();
	~SMaterial() {};
};
