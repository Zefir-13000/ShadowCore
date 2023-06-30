#pragma once
#include "ssi.h"
#include "Object.h"
#include "Shader.h"

class RenderObject : public Object {
public:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	size_t vertices_count = 0, indices_count = 0;
	RenderType render_type;
	HelpObjectType help_type = NONE;
	bool Inited = false;

	std::vector<Vertex> ArrayToVertex(std::vector<float>& _arr);
	std::vector<Vertex> ArrayToVertexPositionOnly(std::vector<float>& _arr);

public:
	virtual ~RenderObject() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Initialize(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, HelpObjectType _help_type);
	void Initialize(std::vector<Vertex>& _vertices, HelpObjectType _help_type);

	RenderObject(std::vector<float>& _vertices, std::vector<unsigned int>& _indices);
	RenderObject(std::vector<float>& _vertices);
	RenderObject(std::vector<Vertex> _vertices, HelpObjectType _help_type);
	RenderObject(std::vector<Vertex>& _vertices);
	RenderObject(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices);
	RenderObject(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, HelpObjectType _help_type);
	RenderObject(std::string _name, std::vector<Vertex> _vertices, HelpObjectType _help_type);
	RenderObject(std::string _name, std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, HelpObjectType _help_type);
	RenderObject(std::string _name, std::vector<float>& _vertices, std::vector<unsigned int>& _indices);
	RenderObject(std::string _name, std::vector<float>& _vertices);
	RenderObject(std::string _name, std::vector<Vertex>& _vertices);
	RenderObject(std::string _name, std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices);

	virtual void Render();

	void SetShader(std::shared_ptr<Shader> _shader);
	std::shared_ptr<Shader> render_shader;
};

