#pragma once
#include "ssi.h"
#include "Object/Mesh.h"
#include "Object/Camera.h"
#include "Common/Material.h"

namespace SC {

	class Level {
	public:
		Level(std::string level_name);
		void Add_Object(std::shared_ptr<Object> object);
		void Destroy_Object(std::shared_ptr<Object> object);

		void Render();

		std::string level_name = "Level";
		std::vector<std::shared_ptr<Object>> objects = {};
		std::vector<std::shared_ptr<Camera>> cameras = {};
		std::vector<std::shared_ptr<Material>> materials = {};
		std::shared_ptr<Camera> main_cam;
	};

};