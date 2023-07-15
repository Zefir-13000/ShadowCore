#include "Core.h"

std::shared_ptr<SC::EngineBase> SC::Core::Engine = nullptr;
std::shared_ptr<SC::Editor> SC::Core::Editor = nullptr;

SC::Core::Core() {
	Engine = std::make_shared<EngineBase>();
}

SC::Core::~Core() {

}