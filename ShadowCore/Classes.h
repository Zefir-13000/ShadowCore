#pragma once

enum ObjectType {
	OBJECT = 0,
	RENDER_OBJECT = 1,
	MESH = 2,
	CAMERA = 3,
	LIGHT = 4
};

enum HelpObjectType {
	NONE = 0,
	AABB_BOX = 1,
	LINE = 2,
};

enum LightType {
	POINT_LIGHT = 0,
	DIRECTIONAL_LIGHT = 1
};

enum MaterialType {
	MATERIAL = 0,
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
	ARRAY = 1,
};

static const char* RenderTypeStr[] = {
	"ELEMENT",
	"ELEMENT_CUSTOM",
	"ARRAY",
	"ARRAY_CUSTOM"
};

enum TextureTypes {
	DIFFUSE = 0,
	SPECULAR = 1,
	EMISSION = 2,
};