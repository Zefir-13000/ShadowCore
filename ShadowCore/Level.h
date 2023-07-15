#pragma once
#include "ssi.h"
#include "Mesh.h"
#include "Camera.h"

namespace SC {

	class Level {
	public:
		Level(std::string level_name);
		void Add_Object(std::shared_ptr<Object> object);

		std::string level_name = "Level";
		std::vector<std::shared_ptr<Object>> objects = {};
		std::vector<std::shared_ptr<Camera>> cameras = {};
		std::shared_ptr<Camera> main_cam;
	};

};