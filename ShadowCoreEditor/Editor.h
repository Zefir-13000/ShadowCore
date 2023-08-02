#pragma once
#include <Core/Core.h>

using namespace SC;

namespace SC::Editor {
	static std::shared_ptr<RenderTexture> viewport = nullptr;
	static uint64_t selected_ObjectID = 0;
}