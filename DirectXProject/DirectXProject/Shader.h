#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

class Shader
{
	private:
	//The vertex Structure
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR   color;
	};


public:
	Shader(void);
	~Shader(void);
};

