#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse_texture;
uniform int has_diffuse_texture;

uniform sampler2D specular_texture;
uniform int has_specular_texture;

uniform vec3 color;

void main()
{
    if (has_diffuse_texture == 1)
        FragColor = texture(diffuse_texture, TexCoords) * vec4(color, 1.0);
    else
        FragColor = vec4(color, 1.0);
} 