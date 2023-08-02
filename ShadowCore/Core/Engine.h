#pragma once
#include <ssi.h>

#include "Core/Level.h"

#include "Object/Object.h"
#include "Common/Shader.h"
#include "Common/Texture.h"
#include "Utils/STime.h"

#include <functional>

namespace SC {

	class EventHandler {
	public:
		static void size_callback(GLFWwindow* window, int width, int height);
	};

	struct Window {
		GLFWwindow* GLFW_window = nullptr;
		int width = 0, height = 0;
	};


	class Engine {
	private:
		using EngineFunctionPtr = std::function<void(Engine*)>;
		EngineFunctionPtr preRenderFunction = &Engine::DefaultPreRender, postRenderFunction = &Engine::DefaultPostRender;
	public:
		Window window;
		std::unique_ptr<EventHandler> eventHandler = std::make_unique<EventHandler>();
		std::shared_ptr<Level> level = std::make_shared<Level>("Level");

		std::shared_ptr<Shader> standart_render_shader = nullptr;
		std::shared_ptr<Shader> debug_shader = nullptr;

		void Init();
		void Init_Shaders();
		void Tick();
		virtual void PostInit();

		virtual void DefaultPreRender();
		virtual void DefaultPostRender();

		void PreRender() {
			preRenderFunction(this);
		}
		void PostRender() {
			postRenderFunction(this);
		}

		virtual void InputProcess();

		
		void SetPreRenderPtr(EngineFunctionPtr functionPtr) {
			preRenderFunction = functionPtr;
		}
		void SetPostRenderPtr(EngineFunctionPtr functionPtr) {
			postRenderFunction = functionPtr;
		}

		Engine();
		~Engine() {}
	};

};