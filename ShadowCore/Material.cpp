#include "Material.h"

MaterialType SC::Material::GetType() {
	return Material::type;
}

SC::SMaterial::SMaterial() {
	SMaterial::type = MATERIAL;
}