#include "Level.h"

Level::Level(std::string _level_name) {
	Level::level_name = _level_name;
}

void Level::Add_Object(std::shared_ptr<Object> _object) {
	Level::objects.push_back(_object);
}