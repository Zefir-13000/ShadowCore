#pragma once

enum ObjectType {
	OBJECT = 1,
	RENDER_OBJECT = 2,
	MESH = 3,
	CAMERA = 4,
	LIGHT = 5
};

enum UseLessType {
	USELESS = 0,
	USEFUL = 1,
	NEEDED = 2,
	MANDATORY = 3,
};

enum ComponentType {
	COMPONENT = 1,
	RENDER_COMPONENT = 2,
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
	MESH_TYPE = 0,
	MODEL_TYPE = 1,
	CONE_TYPE = 2,
	LINE_TYPE = 3,
	POINT_TYPE = 4,
};

enum RenderType {
	ELEMENT = 0,
	ARRAY = 1,
};

static const char* RenderTypeStr[] = {
	"ELEMENT",
	"ARRAY",
};

enum TextureTypes {
	DIFFUSE = 0,
	SPECULAR = 1,
	EMISSION = 2,
};