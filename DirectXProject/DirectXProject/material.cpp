#include "material.h"

#include "shader.h"
#include "texture.h"


Material::Material(void)
{
}

Material::Material(std::string n):
	name(n)
{
}


Material::~Material(void)
{
}


void Material::AddTexture(Texture t)
/*-------------------------------------------------------------------------*\
|	Purpose:	Attach a texture object to the material						|
|																			|
|	Parameters:	The texture to attach										|
\*-------------------------------------------------------------------------*/
{
	textures.push_back(t);
}

void Material::ReplaceTexture(Texture oldTex, Texture newTex)
/*-------------------------------------------------------------------------*\
|	Purpose:	Remove a previously added texture and replace with			|
|				the new texture												|
|																			|
|	Parameters:	A copy of the texture to remove and the texture to replace	|
|				it with														|
\*-------------------------------------------------------------------------*/
{

}

void Material::ReplaceTexture(Texture::Type t, Texture newTex)
/*-------------------------------------------------------------------------*\
|	Purpose:	Remove a previously added texture and replace with			|
|				the new texture												|
|																			|
|	Parameters:	Type of texture to replace and the texture to replace		|
|				it with														|
\*-------------------------------------------------------------------------*/
{

}

void Material::AddShader(std::string shader)
/*-------------------------------------------------------------------------*\
|	Purpose:	Attach a shader object to the material						|
|																			|
|	Parameters:	The file name of the shader to attach						|
\*-------------------------------------------------------------------------*/
{
	Shader newShader(shader);
	shaders.push_back(newShader);
}

std::vector<Texture> Material::GetTextures()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the full list of attached textures					|
|																			|
|	Returns:	The full list of attached textures							|
\*-------------------------------------------------------------------------*/
{
	return textures;
}

std::vector<Shader> Material::GetShaders()
/*-------------------------------------------------------------------------*\
|	Purpose:	Retrieve the full list of attached shaders					|
|																			|
|	Returns:	The full list of attached shaders							|
\*-------------------------------------------------------------------------*/
{
	return shaders;
}
