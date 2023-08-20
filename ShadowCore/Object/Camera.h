#pragma once
#include "ssi.h"
#include "Object/RenderObject.h"
#include "Common/Transform.h"
#include "Utils/STime.h"

namespace SC {

	class Camera : public RenderObject
	{
	private:
		// none
	public:
		bool isActive = true;
		float FOV = 60.f;
		CameraType cam_type = PERSPECTIVE;
		CameraMoveType cam_move_type = STATIC_CAM;

		glm::mat4 view{}, proj{};
		float near_plane = 0.1f, far_plane = 100.f;

		void UpdateProjection(int _width, int _height);
		void UpdateView();
		glm::mat4 GetPVMatrix();
		void SetActive(bool _isActive);

		Camera(int _window_width, int _window_height, float _FOV = 60.f, CameraType _cam_type = PERSPECTIVE);
		virtual ~Camera() {};
	};

	constexpr float CAMERA_NORMAL_SPEED = 5.f;
	constexpr float CAMERA_MAX_SPEED = 15.f;
	class FlyCamera : public Camera {
	public:
		// EDITOR ONLY
		bool isWindowDocked = false;

		bool firstPress = false;
		double lastMouseX = 0, lastMouseY = 0;

		float sensitivity = 0.1f;
		float speed = CAMERA_NORMAL_SPEED;
		void Movement(GLFWwindow* _window, int _width, int _height);

		FlyCamera(int _window_width, int _window_height, float _FOV = 60.f, CameraType _cam_type = PERSPECTIVE) : Camera(_window_width, _window_height, _FOV, _cam_type) {
			cam_move_type = FLY_CAM;
		};
	};

};