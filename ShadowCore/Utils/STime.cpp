#include "STime.h"

using namespace SC;

float STime::deltaTime = 0.f;
float STime::time = 0.f;

void STime::UpdateTime(float current_time) {
	STime::deltaTime = current_time - STime::time;
	STime::time = current_time;
}
