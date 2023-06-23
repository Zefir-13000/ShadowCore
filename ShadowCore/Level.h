#pragma once
#include "ssi.h"
#include "Mesh.h"

class Level {
public:
	Level(std::string level_name);
	void Add_Object(std::shared_ptr<Object> object);

	std::string level_name = "Level";
	std::vector<std::shared_ptr<Object>> objects = {};
};