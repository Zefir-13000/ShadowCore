#pragma once
#include "ssi.h"

#include "Core/Engine.h"
#include "Core/Editor.h"
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
	public:
		static std::shared_ptr<EngineBase> Engine;
		static std::shared_ptr<Editor> Editor;

		Core();
		~Core();
	};

};