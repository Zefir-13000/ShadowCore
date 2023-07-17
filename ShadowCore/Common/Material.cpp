#include "Material.h"
using namespace SC;

MaterialType Material::GetType() {
	return Material::type;
}

SMaterial::SMaterial() {
	SMaterial::type = MATERIAL;
}