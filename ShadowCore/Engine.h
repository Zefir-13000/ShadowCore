#pragma once
#include "ssi.h"

#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Level.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "STime.h"
#include "Ray.h"

class EventHandler {
public:
	static void size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
};

struct Window {
	GLFWwindow* GLFW_window = nullptr;
	int width = 0, height = 0;
};


class EngineBase {
public:
	Window window;
	std::unique_ptr<EventHandler> eventHandler = std::make_unique<EventHandler>();
	std::unique_ptr<Level> level = std::make_unique<Level>("Level");

	std::shared_ptr<Shader> debug_shader = nullptr;

	void Init();
	void Add_Object(std::shared_ptr<Object> object);
	void EngineLoop();
	virtual void PostInit();

	virtual void PreRender();
	virtual void PostRender();
	virtual void InputProcess();


	EngineBase();
	~EngineBase() {}
};
