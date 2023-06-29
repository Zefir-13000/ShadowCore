#pragma once
#include "ssi.h"

constexpr int MAXLEN_OBJECT_NAME = 255;

class Object
{
protected:
	//std::shared_ptr<EngineBase> enginePtr = nullptr;
public:
	
	std::string name;
	ObjectType type = OBJECT;
	Object() {
		Object::name = std::string("Object");
	}
	Object(_In_ std::string name) {
		if (name.size() > 0 && name.size() < MAXLEN_OBJECT_NAME)
			Object::name = name;
		else
			Object::name = std::string("Object");
	}

	//void SetEnginePtr(std::shared_ptr<EngineBase> _enginePtr) {
	//	Object::enginePtr = _enginePtr;
	//}

	virtual ~Object() {}
};

