#pragma once
#include "ssi.h"
#include "Shader.h"

class Texture
{
private:
	unsigned int textureID;
public:
	TextureTypes type = DIFFUSE;
	Texture(std::string name, TextureTypes type);
	void Bind(std::unique_ptr<Shader>& shader, int texNum);
};

