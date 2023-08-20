#pragma once
#include "ssi.h"
#include "Object/Object.h"
#include "Common/Shader.h"
#include "Common/Transform.h"

namespace SC {

	class RenderObject : public Object {
	public:
		static std::shared_ptr<std::vector<Vertex>> ArrayToVertex(std::shared_ptr<std::vector<float>> _arr);
		static std::shared_ptr<std::vector<Vertex>> ArrayToVertexPositionOnly(std::shared_ptr<std::vector<float>> _arr);

		std::shared_ptr<RenderSequence> renderSeq = nullptr;

	public:
		std::shared_ptr<Shader> render_shader;
		std::shared_ptr<ShaderInputCollection> shader_input = nullptr;

		template <class T> void AddShaderInput(std::string& name, const T& value);

		std::shared_ptr<Transform> transform = std::make_shared<Transform>();

		virtual ~RenderObject() {}

		RenderObject(std::string _name, std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);
		RenderObject(std::shared_ptr<GeometryData> _geom_data, UseLessType _u_type);
		RenderObject(std::string _name, std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type);
		RenderObject(std::shared_ptr<RenderSequence> _render_seq, UseLessType _u_type);

		virtual void Render();
		virtual void Render(std::shared_ptr<Shader> _render_shader, bool ignore_inputs = false);
		virtual void RenderComponents();
		virtual void UpdateComponents();

		void SetShader(std::shared_ptr<Shader> _shader);
	};

};