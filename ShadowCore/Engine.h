#pragma once
#include "ssi.h"


#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include "Level.h"
#include "STime.h"

namespace SC {

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

		std::shared_ptr<Shader> standart_render_shader = nullptr;
		std::shared_ptr<Shader> debug_shader = nullptr;

		void Init();
		void Init_Shaders();
		void Add_Object(std::shared_ptr<Object> object);
		void EngineLoop();
		virtual void PostInit();

		virtual void PreRender();
		virtual void PostRender();
		virtual void InputProcess();


		EngineBase();
		~EngineBase() {}
	};

};