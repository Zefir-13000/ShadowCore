#pragma once
#include "ssi.h"

namespace SC {

	class STime
	{
	private:
		STime() {};
		~STime() {};
	public:
		static void UpdateTime(float current_time);

		static float deltaTime;
		static float time;
	};

};