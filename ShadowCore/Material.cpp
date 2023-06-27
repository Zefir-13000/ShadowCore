#include "Material.h"

MaterialType Material::GetType() {
	return Material::type;
}

MaterialColor::MaterialColor() {
	MaterialColor::type = COLOR;
}

MaterialTexture::MaterialTexture(std::shared_ptr<Texture> _diffuse_texture) {
	MaterialTexture::type = TEXTURE;
	MaterialTexture::diffuse_texture = _diffuse_texture;
}