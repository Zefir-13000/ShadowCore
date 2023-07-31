#include "Material.h"
using namespace SC;

MaterialType Material::GetType() {
	return Material::material_type;
}

SMaterial::SMaterial() {
	SMaterial::material_type = SMATERIAL;
}