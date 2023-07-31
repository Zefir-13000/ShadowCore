#include "Texture.h"
#include "Core/Core.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

using namespace SC;

std::string TEXTURE_FOLDER("Textures/");
Texture::Texture() {
    Texture::type = NULL_TEX;
}

Texture::Texture(std::string name, TextureTypes type) {
	int width, height, nrComponents;

	Texture::type = type;
	glGenTextures(1, &textureID);
	
    name = TEXTURE_FOLDER + name;
	std::cout << "Loading texture: " << name << std::endl;
	stbi_set_flip_vertically_on_load(1);

    unsigned char* img = stbi_load(name.c_str(), &width, &height, &nrComponents, 0);
    if (img)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //stbi_image_free(data);
        stbi_image_free(img);
        glBindTexture(GL_TEXTURE_2D, 0);

        Inited = true;
    }
    else
    {
        std::cerr << "ERROR::TEXTURE::LOAD - Texture failed to load at path: " << name << std::endl;
        stbi_image_free(img);
    }
}

void Texture::Bind(std::shared_ptr<Shader> shader, int texNum) {
    if (Inited) {
        shader->Activate();
        if (type == DIFFUSE)
            shader->setInt("has_diffuse_texture", 1);
        else if (type == SPECULAR)
            shader->setInt("has_specular_texture", 1);
        else if (type == EMISSION)
            shader->setInt("has_emission_texture", 1);
        glActiveTexture(GL_TEXTURE0 + texNum);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

uint32_t Texture::GetTextureID() {
    return Texture::textureID;
}

void RenderTexture::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void RenderTexture::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderTexture::Render() {
    RenderTexture::Bind();

    // Draw Scene
    Core::Engine->level->Render();

    RenderTexture::UnBind();
}

void RenderTexture::RecreateFB(int _x, int _y) {
    RenderTexture::size_x = _x;
    RenderTexture::size_y = _y;

    glDeleteRenderbuffers(1, &rbo);
    glDeleteTextures(1, &textureID);
    glDeleteFramebuffers(1, &framebuffer);

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init Color Texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _x, _y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // Init Depth Texture
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _x, _y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::RenderTexture(int _x, int _y, std::shared_ptr<Camera> _render_cam) {
    RenderTexture::size_x = _x;
    RenderTexture::size_y = _y;

    RenderTexture::render_cam = _render_cam;

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init Color Texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _x, _y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // Init Depth Texture
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _x, _y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}