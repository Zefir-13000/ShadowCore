#include "Object.h"
#include "Core.h"

SC::Object::Object() {
	Object::name = std::string("Object");
	enginePtr = Core::Engine;
}