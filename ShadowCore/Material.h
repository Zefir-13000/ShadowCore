#pragma once
#include "ssi.h"
#include "Texture.h"

class Material
{
public:
	MaterialType type = COLOR;

	virtual MaterialType GetType();

	Material() {};
	~Material() {};
};

class MaterialColor : public Material {
public:
	glm::vec3 Albedo{ 0.87, 0.5, 0.27 };
	float Ambient = 0.1f;
	float SpecularStrength = 0.5f;
	float Shininess = 256.f;

	MaterialColor();
	~MaterialColor() {};
};

class MaterialTexture : public Material {
public:
	std::shared_ptr<Texture> diffuse_texture;
	glm::vec3 Albedo{1.};

	MaterialTexture(std::shared_ptr<Texture> _diffuse_texture);
	~MaterialTexture() {};
};

