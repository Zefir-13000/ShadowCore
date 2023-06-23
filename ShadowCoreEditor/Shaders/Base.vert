#version 330 core
layout (location = 0) in vec3 inAPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(inAPos.x, inAPos.y, inAPos.z, 1.0);
    TexCoords = inTexCoords;
}