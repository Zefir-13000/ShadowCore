#pragma once
#include "ssi.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Object.h"

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

	void Init();

	virtual void PreRender() {};
	virtual void PostRender() {};
	virtual void InputProcess() {};

	EngineBase();
	~EngineBase() {}
};
