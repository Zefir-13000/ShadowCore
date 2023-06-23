#pragma once
#include "ssi.h"

#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Level.h"

class EventHandler {
public:
	static void size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
};

class EngineBase {
public:
	GLFWwindow* window;
	std::unique_ptr<EventHandler> eventHandler = std::make_unique<EventHandler>();
	std::unique_ptr<Level> level = std::make_unique<Level>("Level");

	void Init();
	void Add_Object(std::shared_ptr<Object> object);
	virtual void PostInit() {};

	virtual void PreRender() {};
	virtual void PostRender() {};
	virtual void InputProcess() {};

	EngineBase();
	~EngineBase() {}
};
