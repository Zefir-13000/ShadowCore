#include "Core.h"

using namespace SC;

bool Core::isEnableEditor = false;
uint64_t Core::selected_ObjectID = 0;
std::shared_ptr<Engine> Core::Engine = nullptr;

bool Core::Inited = false;

void Core::Init() {
	if (Inited)
		return;

	Core::Engine = std::make_shared<SC::Engine>();
}