#include "Level.h"

using namespace SC;

Level::Level(std::string _level_name = "Level") {
	Level::level_name = _level_name;
}

void Level::Add_Object(std::shared_ptr<Object> _object) {
	Level::objects.push_back(_object);
}

void Level::Destroy_Object(std::shared_ptr<Object> object) {
	Level::objects.erase(std::remove(Level::objects.begin(), Level::objects.end(), object), Level::objects.end());
}