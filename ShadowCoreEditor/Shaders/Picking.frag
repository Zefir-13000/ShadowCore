#version 330 core
out uvec3 FragColor;

uniform uint ObjectIndex;
uniform uint DrawIndex;

void main() {
	FragColor = uvec3(ObjectIndex, 2, gl_PrimitiveID + 1);
}