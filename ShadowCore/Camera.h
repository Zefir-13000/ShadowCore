#pragma once
#include "ssi.h"
#include "Object.h"
#include "Transform.h"
#include "STime.h"

namespace SC {

	class Camera : public Object
	{
	private:
		bool firstPress = false;
		double lastMouseX = 0, lastMouseY = 0;
	public:
		float FOV = 60.f;
		CameraType cam_type = PERSPECTIVE;

		glm::mat4 view{}, proj{};
		float near_plane = 0.1f, far_plane = 100.f;

		Transform local_transform, transform;

		void UpdateProjection(int _width, int _height);
		void UpdateView();

		// movement
		float sensitivity = 0.001f;
		float speed = 5.0f;
		void Movement(GLFWwindow* _window, int _width, int _height);

		Camera(int _window_width, int _window_height, float _FOV = 60.f, CameraType _cam_type = PERSPECTIVE);
	};

};