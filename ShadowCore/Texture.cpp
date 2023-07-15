#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

std::string TEXTURE_FOLDER("Textures/");
SC::Texture::Texture(std::string name, TextureTypes type) {
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
    }
    else
    {
        std::cerr << "ERROR::TEXTURE::LOAD - Texture failed to load at path: " << name << std::endl;
        stbi_image_free(img);
    }
}

void SC::Texture::Bind(std::shared_ptr<Shader> shader, int texNum) {
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