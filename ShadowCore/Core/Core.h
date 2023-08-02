#pragma once
#include "ssi.h"

#include "Core/Engine.h"
#include "Core/Level.h"

#include "Common/Material.h"
#include "Common/Shader.h"
#include "Common/Texture.h"

#include "Object/Object.h"
#include "Object/Mesh.h"
#include "Object/Camera.h"
#include "Object/Light.h"
#include "Object/PrimitiveObjects.h"
#include "Object/Model.h"

#include "Utils/STime.h"

#include "Ray.h"

namespace SC {

	class Core {
	private:
		Core() {};
		~Core() {};

		static bool Inited;
	public:
		//Editor
		static bool isEnableEditor;
		static uint64_t selected_ObjectID;

		static std::shared_ptr<Engine> Engine;

		static void Init();
	};

};