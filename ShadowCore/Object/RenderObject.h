#pragma once
#include "ssi.h"
#include "Object/Object.h"
#include "Common/Shader.h"

namespace SC {

	class RenderObject : public Object {
	public:
		uint32_t VAO = 0, VBO = 0, EBO = 0;
		size_t vertices_count = 0, indices_count = 0;
		RenderType render_type;
		bool Inited = false;

		static std::shared_ptr<std::vector<Vertex>> ArrayToVertex(std::shared_ptr<std::vector<float>> _arr);
		static std::shared_ptr<std::vector<Vertex>> ArrayToVertexPositionOnly(std::shared_ptr<std::vector<float>> _arr);

		std::shared_ptr<std::vector<Vertex>> vertices;
		std::shared_ptr<std::vector<uint32_t>> indices;

	public:
		std::shared_ptr<Shader> render_shader;

		virtual ~RenderObject() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		void Initialize(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices);
		void Initialize(std::shared_ptr<std::vector<Vertex>> _vertices);

		RenderObject(std::shared_ptr<std::vector<float>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices);
		RenderObject(std::shared_ptr<std::vector<float>> _vertices);
		RenderObject(std::shared_ptr<std::vector<Vertex>> _vertices);
		RenderObject(std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices);

		RenderObject(std::string _name, std::shared_ptr<std::vector<float>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices);
		RenderObject(std::string _name, std::shared_ptr<std::vector<float>> _vertices);
		RenderObject(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices);
		RenderObject(std::string _name, std::shared_ptr<std::vector<Vertex>> _vertices, std::shared_ptr<std::vector<uint32_t>> _indices);

		virtual void Render();
		virtual void RenderComponents();

		void SetShader(std::shared_ptr<Shader> _shader);
	};

};