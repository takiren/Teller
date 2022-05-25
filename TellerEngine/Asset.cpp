#include "Asset.h"

using namespace Teller;
void Teller::Sprite::Init()
{
	type = ASSET_TYPE::Image;
	texture = gl::Texture::create(loadImage(filePath));
}

void Teller::Asset::Init()
{
}
