#version 330 core
out uvec3 FragColor;

uniform uint ObjectIndex;

void main() {
	FragColor = uvec3(ObjectIndex, 1, gl_PrimitiveID + 1);
}