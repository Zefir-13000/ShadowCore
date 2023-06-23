#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"

class Mesh : public Object {
private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	unsigned int vertices_count = 0, indices_count = 0;
	RenderType render_type;
	bool Inited = false;
public:
	Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, RenderType render_type);
	Mesh(std::vector<float>& vertices, RenderType render_type);
	Mesh(std::string name, std::vector<float>& vertices, std::vector<unsigned int>& indices, RenderType render_type);
	Mesh(std::string name, std::vector<float>& vertices, RenderType render_type);

	void Render(std::unique_ptr<Shader>& shader);
	void SetTextures(std::vector<std::shared_ptr<Texture>> textures);

	std::vector<std::shared_ptr<Texture>> textures;

	Transform transform;

	~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};