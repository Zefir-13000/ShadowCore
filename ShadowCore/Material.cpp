#include "Material.h"

MaterialType Material::GetType() {
	return Material::type;
}

SMaterial::SMaterial() {
	SMaterial::type = MATERIAL;
}