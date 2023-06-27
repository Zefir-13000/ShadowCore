#pragma once

enum ObjectTypes {
	OBJECT = 0,
	MESH = 1,
	CAMERA = 2,
	LIGHT = 3
};

enum LightType {
	POINT_LIGHT = 0,
	DIRECTIONAL_LIGHT = 1
};

enum MaterialType {
	COLOR = 0,
	TEXTURE = 1,
	COMBINED = 2,
	SHADER = 3
};

enum CameraType {
	PERSPECTIVE = 0,
	ORTHOGRAPHIC = 1
};

enum MeshType {
	MODEL = 0
};

enum RenderType {
	ELEMENT = 0,
	ELEMENT_CUSTOM = 1,
	ARRAY = 2,
	ARRAY_CUSTOM = 3
};

static const char* RenderTypeStr[] = {
	"ELEMENT",
	"ELEMENT_CUSTOM",
	"ARRAY",
	"ARRAY_CUSTOM"
};

enum TextureTypes {
	DIFFUSE = 0,
	SPECULAR = 1
};