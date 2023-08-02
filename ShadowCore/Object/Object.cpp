#include "Object.h"
#include "Core/Core.h"

using namespace SC;

uint64_t Object::Last_id = 0;

Object::Object() {
	enginePtr = Core::Engine;

	Object::name = std::string("Object");

	id = Last_id++;
}

Object::Object(_In_ std::string name) {
	enginePtr = Core::Engine;

	if (name.size() > 0 && name.size() < MAXLEN_OBJECT_NAME)
		Object::name = name;
	else
		Object::name = std::string("Object");

	id = Last_id++;
}

void Object::Destroy() {
	enginePtr->level->Destroy_Object(shared_from_this());
}