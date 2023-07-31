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
		uint32_t framebuffer, rbo;

		std::shared_ptr<Camera> render_cam = nullptr;
	public:
		int size_x = 0, size_y = 0;

		RenderTexture(int x, int y, std::shared_ptr<Camera> _render_cam);
		~RenderTexture() {
			glDeleteRenderbuffers(1, &rbo);
			glDeleteTextures(1, &textureID);
			glDeleteFramebuffers(1, &framebuffer);
		}

		void Bind();
		void UnBind();
		void Render();
		void RecreateFB(int x, int y);
	};

};