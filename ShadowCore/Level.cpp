#include "Level.h"

Level::Level(std::string level_name) {
	Level::level_name = level_name;
}

void Level::Add_Object(std::shared_ptr<Object> object) {
	Level::objects.push_back(object);
}