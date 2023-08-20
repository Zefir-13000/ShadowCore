#include "Camera.h"
#include "Core/Core.h"

using namespace SC;

void Camera::UpdateProjection(int _width, int _height) {
	if (Camera::cam_type == PERSPECTIVE)
		Camera::proj = glm::perspective(glm::radians(Camera::FOV) * static_cast<float>(_width) / static_cast<float>(_height), static_cast<float>(_width) / static_cast<float>(_height), Camera::near_plane, Camera::far_plane);
	else if (Camera::cam_type == ORTHOGRAPHIC)
		Camera::proj = glm::ortho(-Camera::FOV, Camera::FOV, -Camera::FOV, Camera::FOV, Camera::near_plane, Camera::far_plane);
}

void Camera::UpdateView() {
	Camera::view = glm::lookAt(transform->position, transform->position + transform->forward, transform->up);
}

glm::mat4 Camera::GetPVMatrix() {
	return Camera::proj * Camera::view;
}

void Camera::SetActive(bool _isActive) {
	Camera::isActive = _isActive;
}

Camera::Camera(int _window_width, int _window_height, float _FOV, CameraType _cam_type) : RenderObject("Camera", std::make_shared<GeometryData>(), MANDATORY) {
	Camera::FOV = _FOV;
	Camera::cam_type = _cam_type;
	Camera::type = CAMERA;

	UpdateView();
	UpdateProjection(_window_width, _window_height);
}

void FlyCamera::Movement(GLFWwindow* _window, int _width, int _height)
{
	if (!GetAsyncKeyState(VK_LBUTTON) && GetAsyncKeyState(VK_RBUTTON) && firstPress && Camera::isActive) {
		// Fly controll
		if (GetAsyncKeyState(VK_LSHIFT)) {
			FlyCamera::speed = CAMERA_MAX_SPEED;
		}
		else {
			FlyCamera::speed = CAMERA_NORMAL_SPEED;
		}

		if (GetAsyncKeyState('W'))
			transform->Translate(transform->position + transform->forward * speed * STime::deltaTime);
		if (GetAsyncKeyState('S'))
			transform->Translate(transform->position - transform->forward * speed * STime::deltaTime);
		if (GetAsyncKeyState('D'))
			transform->Translate(transform->position + transform->right * speed * STime::deltaTime);
		if (GetAsyncKeyState('A'))
			transform->Translate(transform->position - transform->right * speed * STime::deltaTime);
		if (GetAsyncKeyState(VK_SPACE))
			transform->Translate(transform->position + transform->up * speed * STime::deltaTime);
		if (GetAsyncKeyState(VK_LCONTROL))
			transform->Translate(transform->position - transform->up * speed * STime::deltaTime);


		// Look
		if (Core::isEnableEditor) {
			POINT p;
			if (GetCursorPos(&p)) {

				float deltaY = sensitivity * static_cast<float>(p.x - lastMouseX);
				float deltaX = sensitivity * static_cast<float>(p.y - lastMouseY);

				transform->Rotate(glm::vec3(transform->rotation.x - deltaX, transform->rotation.y - deltaY, transform->rotation.z));

				SetCursorPos(static_cast<int>(lastMouseX), static_cast<int>(lastMouseY));

			}
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		}
		else {
			double mouseX;
			double mouseY;

			glfwGetCursorPos(_window, &mouseX, &mouseY);
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			float deltaY = sensitivity * static_cast<float>(mouseX - lastMouseX);
			float deltaX = sensitivity * static_cast<float>(mouseY - lastMouseY);

			transform->Rotate(glm::vec3(transform->rotation.x - deltaX, transform->rotation.y - deltaY, transform->rotation.z));

			glfwSetCursorPos(_window, static_cast<float>(_width) / 2.f, static_cast<float>(_height) / 2.f);
			glfwGetCursorPos(_window, &lastMouseX, &lastMouseY);
		}
			
	}
	else if (Camera::isActive && !GetAsyncKeyState(VK_LBUTTON) && GetAsyncKeyState(VK_RBUTTON) && !firstPress) {
		if (Core::isEnableEditor) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			SetCursorPos(static_cast<int>(lastMouseX), static_cast<int>(lastMouseY));
		}
		else {
			glfwSetCursorPos(_window, static_cast<float>(_width) / 2.f, static_cast<float>(_height) / 2.f);
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			glfwGetCursorPos(_window, &lastMouseX, &lastMouseY);
		}
		firstPress = true;
	}
	else if (GetAsyncKeyState(VK_RBUTTON) == 0) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstPress = false;
	}

	UpdateView();
}