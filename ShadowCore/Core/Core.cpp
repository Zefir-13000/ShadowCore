#include "Core.h"

using namespace SC;

std::shared_ptr<EngineBase> Core::Engine = nullptr;
std::shared_ptr<Editor> Core::Editor = nullptr;

bool Core::Inited = false;

void Core::Init() {
	if (Inited)
		return;

	Engine = std::make_shared<EngineBase>();
}