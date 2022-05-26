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
void Episode::SetLineBegin(int line)
{
	line_begin = line;
}

void Episode::SetLineEnd(int line)
{
	line_end = line;
}

void Episode::SetNumber(int episodeNumber)
{
}