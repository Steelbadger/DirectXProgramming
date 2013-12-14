#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include "GameObject.h"

class ForwardShaderInterface
{
public:
	ForwardShaderInterface(){};
	virtual ~ForwardShaderInterface(){};
	
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(ID3D11DeviceContext* dc, ObjectID drawObject, ObjectID cameraObject, ObjectID light) = 0;
};