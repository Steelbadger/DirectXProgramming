#include "ShaderLibrary.h"

#include "lightshaderclass.h"
#include "NormalMapShaderClass.h"



ShaderLibrary::ShaderLibrary()
{

}


ShaderLibrary::~ShaderLibrary()
{

}


void ShaderLibrary::Initialise(ID3D11Device* dev, HWND handle)
{
	shaders[LIT] = new LightShaderClass;
	shaders[LIT]->Initialize(dev, handle);

	shaders[NORMAL] = new NormalMapShaderClass;
	shaders[NORMAL]->Initialize(dev, handle);
}


ForwardShaderInterface* ShaderLibrary::GetShader(Shaders s)
{
	if (shaders.count(s) > 0) {
		return shaders[s];
	} else {
		return nullptr;
	}
}