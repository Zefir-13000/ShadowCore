#pragma once
#include <Core/Core.h>

using namespace SC;

namespace SC::Editor {
	static std::shared_ptr<RenderTexture> viewport = nullptr;
	static std::shared_ptr<PickingTexture> pickingTexture = nullptr;
	static std::shared_ptr<Shader> pickingShader = nullptr;
	static uint64_t selected_ObjectID = 0;

	void PickingPhase();

	static struct MouseLocalPos {
		uint32_t x, y;
	} MousePos;
}