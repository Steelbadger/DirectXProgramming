#pragma once
#include <map>

#include "ShaderInterface.h"

class ShaderLibrary
{
public:
	enum Shaders {LIT, NORMAL};
	ShaderLibrary();
	~ShaderLibrary();

	void Initialise(ID3D11Device*, HWND);
	
	ForwardShaderInterface* GetShader(Shaders s);

private:
	std::map<Shaders, ForwardShaderInterface*> shaders;
};