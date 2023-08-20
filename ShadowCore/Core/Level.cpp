#include "Level.h"
#include "Core/Core.h"

using namespace SC;

Level::Level(std::string _level_name = "Level") {
	Level::level_name = _level_name;
}

std::shared_ptr<Object> Level::GetObjectByID(uint32_t id) {
	for (std::shared_ptr<Object> obj : Level::objects) {
		if (obj->getId() == id) {
			return obj;
		}
		for (std::shared_ptr<Object> child : std::dynamic_pointer_cast<RenderObject>(obj)->transform->children) {
			if (child->getId() == id) {
				return child;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<Object> Level::Add_Object(std::shared_ptr<Object> _object) {
	uint32_t id = _object->getId();
	for (std::shared_ptr<Object> obj : Level::objects) {
		if (id == obj->getId()) {
			std::cout << "ERROR::LEVEL::ADD_OBJECT id(" << id << ") name(" << _object->name << ") - TRY TO ADD OBJECT WITH SAME ID TWICE! existing_id(" << obj->getId() << ") name(" << obj->name << ")" << std::endl;
			return nullptr;
		}
	}

	Level::objects.push_back(_object);
	return _object;
}


void Level::Add_Shadow(std::shared_ptr<Camera> _render_cam, uint32_t _shadow_size) {
	std::shared_ptr<ShadowMapTexture> shadow_map = std::make_shared<ShadowMapTexture>(_render_cam, _shadow_size);
	Level::shadows.push_back(shadow_map);
}

void Level::Destroy_Object(std::shared_ptr<Object> object) {
	Level::objects.erase(std::remove(Level::objects.begin(), Level::objects.end(), object), Level::objects.end());
}

void Level::Render() {
	for (std::shared_ptr<Object> object : objects) {
		if (object->type == RENDER_OBJECT || object->type == MESH) {
			std::shared_ptr<RenderObject> rd = std::dynamic_pointer_cast<RenderObject>(object);

			rd->Render();
		}
	}
}

void Level::Render(std::shared_ptr<Shader> _render_shader) {
	for (std::shared_ptr<Object> object : objects) {
		if (object->type == RENDER_OBJECT || object->type == MESH) {
			std::shared_ptr<RenderObject> rd = std::dynamic_pointer_cast<RenderObject>(object);

			rd->Render(_render_shader);
		}
	}
}