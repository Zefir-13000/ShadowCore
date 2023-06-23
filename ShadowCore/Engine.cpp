#include "Engine.h"

EngineBase::EngineBase() {
	Init();
}

void EngineBase::Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, &eventHandler);
	glfwSetFramebufferSizeCallback(window, &EventHandler::size_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to init GLEW!" << std::endl;
		return;
	}
}

void EngineBase::Add_Object(std::shared_ptr<Object> object) {
	level->Add_Object(object);
}