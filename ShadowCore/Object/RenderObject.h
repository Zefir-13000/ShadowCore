#pragma once
#include "ssi.h"
#include "Object/Object.h"
#include "Common/Shader.h"
#include "Common/Transform.h"

namespace SC {

	class RenderObject : public Object {
	public:
		uint32_t VAO = 0, VBO = 0, EBO = 0;
		bool Inited = false;

		static std::shared_ptr<std::vector<Vertex>> ArrayToVertex(std::shared_ptr<std::vector<float>> _arr);
		static std::shared_ptr<std::vector<Vertex>> ArrayToVertexPositionOnly(std::shared_ptr<std::vector<float>> _arr);

		std::shared_ptr<GeometryData> geometry_data = nullptr;

	public:
		std::shared_ptr<Shader> render_shader;

		Transform local_transform, transform;

		virtual ~RenderObject() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
		}

		void Initialize(std::shared_ptr<GeometryData> _geom_data);
		void UnInitialize();

		RenderObject(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);
		RenderObject(std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);

		virtual void Render();
		virtual void RenderComponents();

		void SetShader(std::shared_ptr<Shader> _shader);
	};

};