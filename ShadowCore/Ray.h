#pragma once
#include "ssi.h"
#include "RayObjects.h"

namespace SC {

	struct RayHitInfo {
		glm::vec3 position;
		glm::vec3 normal;
	};

	class Ray
	{
	public:
		glm::vec3 origin;
		glm::vec3 direction;

		Ray(glm::vec3 origin, glm::vec3 direction);

		static glm::vec3 GetScreenToWorld(glm::vec2 mousePos, glm::vec2 ScreenSize, const glm::mat4& ProjectionMatrix, const glm::mat4& ViewMatrix, const glm::vec3& cameraPosition);
		float RayIntersectsAABB(const AABB_Box& box, const glm::mat4& modelMatrix);
	};

};