#pragma once

enum ObjectTypes {
	OBJECT = 0x00,
	MESH = 0x01
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
	DIFFUSE = 0x00,
	SPECULAR = 0x01
};