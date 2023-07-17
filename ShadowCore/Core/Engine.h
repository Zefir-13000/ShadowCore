#pragma once
#include <ssi.h>

#include "Core/Level.h"

#include "Object/Object.h"
#include "Common/Shader.h"
#include "Common/Texture.h"
#include "Utils/STime.h"

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
		std::shared_ptr<Level> level = std::make_shared<Level>("Level");

		std::shared_ptr<Shader> standart_render_shader = nullptr;
		std::shared_ptr<Shader> debug_shader = nullptr;

		void Init();
		void Init_Shaders();
		void EngineLoop();
		virtual void PostInit();

		virtual void PreRender();
		virtual void PostRender();
		virtual void InputProcess();


		EngineBase();
		~EngineBase() {}
	};

};