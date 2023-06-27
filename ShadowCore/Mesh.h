#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Material.h"

class Mesh : public Object {
private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	size_t vertices_count = 0, indices_count = 0;
	RenderType render_type;
	bool Inited = false;

	std::vector<Vertex> ArrayToVertex(std::vector<float>& _arr);
public:
	MeshType meshType = MODEL;

	Mesh(std::vector<float>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type);
	Mesh(std::vector<float>& _vertices, RenderType _render_type);
	Mesh(std::vector<Vertex>& _vertices, RenderType _render_type);
	Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type);
	Mesh(std::string _name, std::vector<float>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type);
	Mesh(std::string _name, std::vector<float>& _vertices, RenderType _render_type);
	Mesh(std::string _name, std::vector<Vertex>& _vertices, RenderType _render_type);
	Mesh(std::string _name, std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, RenderType _render_type);

	void Render();
	void SetShader(std::shared_ptr<Shader> _shader);
	void SetMaterial(std::shared_ptr<Material> _material);

	std::shared_ptr<Material> material;

	Transform local_transform, transform;

	std::shared_ptr<Shader> render_shader;

	~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};