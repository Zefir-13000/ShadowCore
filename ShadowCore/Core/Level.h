#pragma once
#include "ssi.h"
#include "Object/Mesh.h"
#include "Object/Camera.h"
#include "Common/Material.h"

namespace SC {

	class Level {
	public:
		Level(std::string level_name);
		std::shared_ptr<Object> GetObjectByID(uint32_t id);
		std::shared_ptr<Object> Add_Object(std::shared_ptr<Object> object);
		template <class T, typename... Args>
		std::shared_ptr<Object> Add_Object(Args... args) {
			std::shared_ptr<Object> object = std::make_shared<T>(args...);

			Level::objects.push_back(object);
			return object;
		}

		void Add_Shadow(std::shared_ptr<Camera> _render_cam, uint32_t _shadow_size = 512); // TODO: Rename to Add_ShadowCascade after implementing cascade shadow maps
		void Destroy_Object(std::shared_ptr<Object> object);

		void Render();
		void Render(std::shared_ptr<Shader> _render_shader);

		std::string level_name = "Level";
		std::vector<std::shared_ptr<Object>> objects = {};
		std::vector<std::shared_ptr<Object>> temp_objects = {};
		std::vector<std::shared_ptr<Camera>> cameras = {};
		std::vector<std::shared_ptr<Material>> materials = {};
		std::shared_ptr<Camera> main_cam;

		std::vector < std::shared_ptr<ShadowMapTexture>> shadows = {};
	};

};