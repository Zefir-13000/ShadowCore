#pragma once
#include "ssi.h"
#include "Engine.h"

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
#include "PrimitiveObjects.h"
#include "Model.h"
#include "Editor.h"

namespace SC {

	class Core {
	public:
		static std::shared_ptr<EngineBase> Engine;
		static std::shared_ptr<Editor> Editor;

		Core();
		~Core();
	};

};