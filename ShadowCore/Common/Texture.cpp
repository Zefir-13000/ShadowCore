#include "Texture.h"
#include "Core/Core.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

using namespace SC;

std::string TEXTURE_FOLDER("Textures/");
Texture::Texture() {
    Texture::type = NULL_TEXTURE;
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
        Texture::size_x = width;
        Texture::size_y = height;

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
        if (type == DIFFUSE_TEXTURE)
            shader->setValue("has_diffuse_texture", 1);
        else if (type == SPECULAR_TEXTURE)
            shader->setValue("has_specular_texture", 1);
        else if (type == EMISSION_TEXTURE)
            shader->setValue("has_emission_texture", 1);
        glActiveTexture(GL_TEXTURE0 + texNum);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

uint32_t Texture::GetTextureID() {
    return Texture::textureID;
}

void RenderTexture::Bind() {
    if (Inited) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }
}

void RenderTexture::UnBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderTexture::Render(bool autoBind) {
    if (Inited && autoBind) {
        RenderTexture::Bind();

        // Draw Scene
        Core::Engine->level->Render();

        RenderTexture::UnBind();
    }
    else if (Inited && !autoBind) {
        // Draw Scene
        Core::Engine->level->Render();
    }
}

void RenderTexture::RecreateFB(uint32_t _x, uint32_t _y) {
    Inited = false;

    RenderTexture::size_x = _x;
    RenderTexture::size_y = _y;

    glDeleteRenderbuffers(1, &depth_tex);
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
    glGenRenderbuffers(1, &depth_tex);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_tex);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _x, _y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_tex);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

RenderTexture::RenderTexture(std::shared_ptr<Camera> _render_cam, uint32_t _x = 1, uint32_t _y = 1) {
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
    glGenRenderbuffers(1, &depth_tex);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_tex);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _x, _y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_tex);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

ShadowMapTexture::ShadowMapTexture(std::shared_ptr<Camera> _render_cam, uint32_t _size) {
    ShadowMapTexture::size_x = _size;
    ShadowMapTexture::size_y = _size;

    ShadowMapTexture::render_cam = _render_cam;

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init depth texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        _size, _size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

void ShadowMapTexture::Bind(int tex_num, bool enable_framebuffer) {
    if (Inited) {
        if (enable_framebuffer)
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glActiveTexture(GL_TEXTURE0 + tex_num);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}
void ShadowMapTexture::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void ShadowMapTexture::Render(int tex_num) {
    ShadowMapTexture::Bind(tex_num);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ShadowMapTexture::size_x, ShadowMapTexture::size_y);
    // Draw Scene
    Core::Engine->level->Render(Core::Engine->shadow_shader);

    glViewport(0, 0, Core::Engine->window.width, Core::Engine->window.height);

    ShadowMapTexture::UnBind();
}
void ShadowMapTexture::RecreateFB(uint32_t _size) {
    Inited = false;

    glDeleteTextures(1, &textureID);
    glDeleteFramebuffers(1, &framebuffer);

    ShadowMapTexture::size_x = _size;
    ShadowMapTexture::size_y = _size;

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init depth texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        _size, _size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

PickingTexture::PickingTexture(uint32_t _x = 1, uint32_t _y = 1) {
    PickingTexture::size_x = _x;
    PickingTexture::size_y = _y;

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init picking data texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, _x, _y, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // create depth texture
    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _x, _y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

void PickingTexture::Bind() {
    if (Inited)
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
}

void PickingTexture::UnBind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void PickingTexture::RecreateFB(uint32_t _x, uint32_t _y) {
    Inited = false;

    PickingTexture::size_x = _x;
    PickingTexture::size_y = _y;

    glDeleteRenderbuffers(1, &depth_tex);
    glDeleteTextures(1, &textureID);
    glDeleteFramebuffers(1, &framebuffer);

    // Init FrameBuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Init picking data texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, _x, _y, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

    // create depth texture
    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _x, _y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Inited = true;
}

PickingTexture::PixelInfo PickingTexture::ReadPixel(uint32_t _x, uint32_t _y) {
    if (Inited) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);

        glReadBuffer(GL_COLOR_ATTACHMENT0);

        PixelInfo Pixel{};
        glReadPixels(_x, _y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &Pixel);

        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        return Pixel;
    }
    return {};
}