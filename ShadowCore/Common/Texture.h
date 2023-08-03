#pragma once
#include "ssi.h"
#include "Common/Shader.h"

namespace SC {

	class Texture
	{
	protected:
		uint32_t textureID = NULL;
		bool Inited = false;
	public:
		uint32_t size_x = 0, size_y = 0;

		TextureTypes type = DIFFUSE;
		Texture(std::string name, TextureTypes type);
		Texture();
		virtual ~Texture() {
			glDeleteTextures(1, &textureID);
		}
		virtual void Bind(std::shared_ptr<Shader> shader, int texNum);
		uint32_t GetTextureID();
	};

	class Level;
	class Camera;
	class RenderTexture : public Texture {
	private:
		uint32_t framebuffer, depth_tex;

		std::shared_ptr<Camera> render_cam = nullptr;
	public:
		RenderTexture(std::shared_ptr<Camera> _render_cam, uint32_t _x, uint32_t _y);
		~RenderTexture() {
			glDeleteRenderbuffers(1, &depth_tex);
			glDeleteTextures(1, &textureID);
			glDeleteFramebuffers(1, &framebuffer);
		}

		void Bind();
		void UnBind();
		void Render();
		void RecreateFB(uint32_t _x, uint32_t _y);
	};

	class PickingTexture : public Texture {
	private:
		uint32_t framebuffer, depth_tex;

	public:
		struct PixelInfo {
			uint32_t ObjectID = 0;
			uint32_t DrawID = 0;
			uint32_t PrimID = 0;
		};

		PickingTexture(uint32_t _x, uint32_t _y);
		~PickingTexture() {
			glDeleteRenderbuffers(1, &depth_tex);
			glDeleteTextures(1, &textureID);
			glDeleteFramebuffers(1, &framebuffer);
		}

		PixelInfo ReadPixel(uint32_t _x, uint32_t _y);

		void RecreateFB(uint32_t _x, uint32_t _y);

		void Bind();
		void UnBind();
	};

};