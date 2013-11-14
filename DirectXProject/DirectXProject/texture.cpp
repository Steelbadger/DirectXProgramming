#include "texture.h"


Texture::Texture(void)
{
}

Texture::Texture(Type t, std::string p)
{
	SetTexture(t, p);
}


Texture::~Texture(void)
{
}

void Texture::SetTexture(Type t, std::string p)
/*-------------------------------------------------------------------------*\
|	Purpose:	Create a texture using a file at location p					|
|																			|
|	Parameters:	The type of the texture and the pathname of the texture file|
\*-------------------------------------------------------------------------*/
{
	type = t;
	path = p;
}


void Texture::SetLoaded(bool isLoaded)
{
	loaded = isLoaded;
}

void Texture::IsLoaded()
{
	return loaded;
}