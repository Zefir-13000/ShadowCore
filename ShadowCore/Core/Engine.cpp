#include "Engine.h"
#include "Core/Core.h"

using namespace SC;

void EventHandler::size_callback(GLFWwindow* window, int width, int height) {
	if (!Core::isEnableEditor)
		glViewport(0, 0, width, height);
}

Engine::Engine() : preRenderFunction(std::bind(&Engine::DefaultPreRender, this)) {
	Init();
	Init_Shaders();
	PostInit();
}

void Engine::Init_Shaders() {
	Engine::standart_render_shader = std::make_shared<SC::Shader>("Base");
	Engine::debug_shader = std::make_shared<SC::Shader>("Debug");
}

void Engine::Init() {
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
		Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
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

void Engine::DefaultPreRender() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	level->Render();

	Engine::Tick();
}

void Engine::DefaultPostRender() {

}

void Engine::Tick()
{
	STime::UpdateTime(static_cast<float>(glfwGetTime()));
}
