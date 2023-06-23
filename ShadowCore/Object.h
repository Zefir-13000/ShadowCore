#pragma once
#include "ssi.h"

constexpr int MAXLEN_OBJECT_NAME = 255;

class Object
{
public:
	std::string name;
	ObjectTypes type = OBJECT;
	Object() {
		this->name = std::string("Object");
	}
	Object(_In_ std::string name) {
		if (name.size() > 0 && name.size() < MAXLEN_OBJECT_NAME)
			this->name = name;
		else
			this->name = std::string("Object");
	}
	virtual ~Object() {}
};

