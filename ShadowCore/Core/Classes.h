#pragma once

enum ObjectType {
	OBJECT = 1,
	RENDER_OBJECT = 2,
	MESH = 3,
	CAMERA = 4,
	LIGHT = 5
};

// Object usefulness
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
	NOLIGHT = 0,
	POINT_LIGHT = 1,
	DIRECTIONAL_LIGHT = 2
};

enum MaterialType {
	MATERIAL = 0,
	SMATERIAL = 1,
	SHADER = 3
};

enum CameraType {
	PERSPECTIVE = 0,
	ORTHOGRAPHIC = 1
};

enum CameraMoveType {
	STATIC_CAM = 0,
	FLY_CAM = 1
};

enum MeshType {
	MESH_TYPE = 0,
	MODEL_TYPE = 1,
	CONE_TYPE = 2,
	LINE_TYPE = 3,
	POINT_TYPE = 4,
	CUBE_TYPE = 5,
	ARROW_TYPE = 6,
	PLANE_TYPE = 7,
};

enum RenderType {
	NORENDER = 0,
	ELEMENT = 1,
	ARRAY = 2,
};

static const char* RenderTypeStr[] = {
	"ELEMENT",
	"ARRAY",
};

enum TextureTypes {
	NULL_TEXTURE = 0,
	DIFFUSE_TEXTURE = 1,
	SPECULAR_TEXTURE = 1,
	EMISSION_TEXTURE = 2,
	RENDER_TEXTURE = 3,
	PICKING_TEXTURE = 4,
};