#include "Object.h"
#include "Core/Core.h"

using namespace SC;

Object::Object() {
	Object::name = std::string("Object");
	enginePtr = Core::Engine;
}

void Object::Destroy() {
	enginePtr->level->Destroy_Object(shared_from_this());
}