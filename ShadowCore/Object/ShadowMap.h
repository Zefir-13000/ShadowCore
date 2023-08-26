#pragma once
#include "ssi.h"
#include "Camera.h"

constexpr uint32_t SHADOW_1k = 1024;
constexpr uint32_t SHADOW_2k = 2048;
constexpr uint32_t SHADOW_4k = 4096;
constexpr uint32_t SHADOW_8k = 8192;

namespace SC {

	class DirectionalLight;
	class CascadeShadowMap {
	private:
		std::vector<float> shadowCascadeLevels = {};

		uint32_t framebuffer, depth_tex, shadow_map_size;

		std::shared_ptr<DirectionalLight> dir_light = nullptr;
	public:
		glm::mat4 getLightSpaceMatrix(uint32_t cascade_index, const float nearPlane, const float farPlane);
		std::vector<glm::mat4> getLightMatrices();

		const std::vector<float>& GetCascadeLevels() const {
			return shadowCascadeLevels;
		}

		CascadeShadowMap(std::shared_ptr<DirectionalLight> _dir_light, uint32_t _size = SHADOW_1k);
		~CascadeShadowMap();

		void Update();
		void Bind();
		void UnBind();
		void Render();
	};

};