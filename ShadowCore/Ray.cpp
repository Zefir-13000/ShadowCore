#include "Ray.h"

using namespace SC;

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
	Ray::origin = origin;
	Ray::direction = direction;
}

glm::vec3 Ray::GetScreenToWorld(glm::vec2 mousePos, glm::vec2 ScreenSize, const glm::mat4& ProjectionMatrix, const glm::mat4& ViewMatrix, const glm::vec3& cameraPosition) {
	glm::mat4 invMat = glm::inverse(ProjectionMatrix * ViewMatrix);
	glm::vec4 near_plane = glm::vec4((mousePos.x - (ScreenSize.x / 2)) / (ScreenSize.x / 2), -1 * (mousePos.y - (ScreenSize.y / 2)) / (ScreenSize.y / 2), -1, 1.0);
	glm::vec4 far_plane = glm::vec4((mousePos.x - (ScreenSize.x / 2)) / (ScreenSize.x / 2), -1 * (mousePos.y - (ScreenSize.y / 2)) / (ScreenSize.y / 2), 1, 1.0);
	glm::vec4 nearResult = invMat * near_plane;
	glm::vec4 farResult = invMat * far_plane;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	glm::vec3 dir = glm::vec3(farResult - nearResult);
	return glm::normalize(dir);
}

float Ray::RayIntersectsAABB(const AABB_Box& box, const glm::mat4& modelMatrix) {
	float t1 = (box.min.x - Ray::origin.x) / Ray::direction.x;
	float t2 = (box.max.x - Ray::origin.x) / Ray::direction.x;
	float t3 = (box.min.y - Ray::origin.y) / Ray::direction.y;
	float t4 = (box.max.y - Ray::origin.y) / Ray::direction.y;
	float t5 = (box.min.z - Ray::origin.z) / Ray::direction.z;
	float t6 = (box.max.z - Ray::origin.z) / Ray::direction.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	if (tmax < 0) {
		return -1;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax) {
		return -1;
	}

	if (tmin < 0.f) {
		return tmax;
	}
	return tmin;
}
