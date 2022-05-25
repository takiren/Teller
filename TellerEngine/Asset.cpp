#include "Asset.h"

using namespace Teller;
using namespace ci;
using namespace ci::app;

void Teller::Sprite::Init()
{
	type = ASSET_TYPE::Image;
	try
	{
		texture = gl::Texture::create(loadImage(filePath));
	}
	catch (Exception &exc)
	{
		CI_LOG_EXCEPTION("failed to load image.", exc);
	}
}

void Teller::Asset::Init()
{
}
