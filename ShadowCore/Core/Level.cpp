#include "Level.h"
#include "Core/Core.h"

using namespace SC;

Level::Level(std::string _level_name = "Level") {
	Level::level_name = _level_name;
}

void Level::Add_Object(std::shared_ptr<Object> _object) {
	uint64_t id = _object->getId();
	for (std::shared_ptr<Object> obj : Level::objects) {
		if (id == obj->getId()) {
			std::cout << "ERROR::LEVEL::ADD_OBJECT id(" << id << ") name(" << _object->name << ") - TRY TO ADD OBJECT WITH SAME ID TWICE! existing_id(" << obj->getId() << ") name(" << obj->name << ")" << std::endl;
			return;
		}
	}
	Level::objects.push_back(_object);
}

template <class T, class... Args> 
void Level::Add_Object(Args&&... args) {
	std::shared_ptr<Object> object = std::make_shared<T>();
	if (Core::isEnableEditor)
		object->AddComponent<AABB>();
	Level::objects.push_back(object);
}

void Level::Destroy_Object(std::shared_ptr<Object> object) {
	Level::objects.erase(std::remove(Level::objects.begin(), Level::objects.end(), object), Level::objects.end());
}

void Level::Render() {
	for (std::shared_ptr<Object> object : objects) {
		if (object->type == MESH) {
			std::shared_ptr<Mesh> rd = std::dynamic_pointer_cast<Mesh>(object);

			rd->Render();
		}
	}
}