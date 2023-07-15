#include "Engine.h"

void SC::EngineBase::Init_Shaders() {
	EngineBase::standart_render_shader = std::make_shared<SC::Shader>("Base");
	EngineBase::debug_shader = std::make_shared<SC::Shader>("Debug");
}

void SC::EngineBase::Init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window.GLFW_window = glfwCreateWindow(1280, 720, "ShadowCore Engine", NULL, NULL);
	window.width = 1280;
	window.height = 720;
	if (window.GLFW_window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window.GLFW_window);

	glfwSetWindowUserPointer(window.GLFW_window, this);
	glfwSetFramebufferSizeCallback(window.GLFW_window, [](GLFWwindow* window, int width, int height) {
		EngineBase* engine = static_cast<EngineBase*>(glfwGetWindowUserPointer(window));
		engine->window.width = width;
		engine->window.height = height;

		engine->level->main_cam->UpdateProjection(width, height);
		for (std::shared_ptr<Camera> camera : engine->level->cameras) {
			camera->UpdateProjection(width, height);
		}

		EventHandler::size_callback(window, width, height);
	});

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to init GLEW!" << std::endl;
		return;
	}
}

void SC::EngineBase::Add_Object(std::shared_ptr<Object> object) {
	if (object != nullptr)
		level->Add_Object(object);
}

void SC::EngineBase::EngineLoop()
{
	STime::UpdateTime(static_cast<float>(glfwGetTime()));
}
