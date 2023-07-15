#include "STime.h"

float SC::STime::deltaTime = 0.f;
float SC::STime::time = 0.f;

void SC::STime::UpdateTime(float current_time) {
	STime::deltaTime = current_time - STime::time;
	STime::time = current_time;
}
