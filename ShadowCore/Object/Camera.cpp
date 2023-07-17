#include "Camera.h"

void SC::Camera::UpdateProjection(int _width, int _height) {
	Camera::proj = glm::perspective(glm::radians(Camera::FOV), static_cast<float>(_width) / static_cast<float>(_height), Camera::near_plane, Camera::far_plane);
}

void SC::Camera::UpdateView() {
	Camera::view = glm::lookAt(transform.position, transform.position + transform.forward, transform.up);
}

void SC::Camera::Movement(GLFWwindow* _window, int _width, int _height)
{
	if (glfwGetMouseButton(_window, 1) == GLFW_PRESS && firstPress) {
		// Fly
		if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
			transform.Translate(transform.position + transform.forward * speed * STime::deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
			transform.Translate(transform.position - transform.forward * speed * STime::deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
			transform.Translate(transform.position + transform.right * speed * STime::deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
			transform.Translate(transform.position - transform.right * speed * STime::deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			transform.Translate(transform.position + transform.up * speed * STime::deltaTime);
		if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			transform.Translate(transform.position - transform.up * speed * STime::deltaTime);


		// Look
		double mouseX;
		double mouseY;
		glfwGetCursorPos(_window, &mouseX, &mouseY);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		float deltaY = sensitivity * static_cast<float>(mouseX - lastMouseX);
		float deltaX = sensitivity * static_cast<float>(mouseY - lastMouseY);

		transform.Rotate(glm::vec3(transform.rotation.x - deltaX, transform.rotation.y - deltaY, transform.rotation.z));

		glfwSetCursorPos(_window, static_cast<float>(_width) / 2.f, static_cast<float>(_height) / 2.f);
		glfwGetCursorPos(_window, &lastMouseX, &lastMouseY);
	}
	else if (glfwGetMouseButton(_window, 1) == GLFW_PRESS && !firstPress) {
		glfwSetCursorPos(_window, static_cast<float>(_width) / 2.f, static_cast<float>(_height) / 2.f);
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwGetCursorPos(_window, &lastMouseX, &lastMouseY);
		firstPress = true;
	}
	else if (glfwGetMouseButton(_window, 1) == GLFW_RELEASE) {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstPress = false;
	}

	UpdateView();
}

SC::Camera::Camera(int _window_width, int _window_height, float _FOV, CameraType _cam_type) {
	Camera::FOV = _FOV;
	Camera::cam_type = _cam_type;
	Camera::type = CAMERA;

	UpdateView();
	UpdateProjection(_window_width, _window_height);
}