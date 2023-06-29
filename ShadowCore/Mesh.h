#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Material.h"
#include "RenderObject.h"
#include "RayObjects.h"

class Mesh : public RenderObject {
private:
	// none
public:
	MeshType meshType = MODEL;

	void SetMaterial(std::shared_ptr<Material> _material);

	std::shared_ptr<Material> material;

	void Render();

	Mesh(std::string _name, std::vector<float>& _vertices);

	Transform local_transform, transform;
	std::shared_ptr<AABB> aabb_box = nullptr;
};