#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class DirectionalLight : public Component<DirectionalLight>
{
public:
	DirectionalLight(){};
	~DirectionalLight(){};
	void SetColour(float r, float g, float b, float a){m_Colour = D3DXVECTOR4(r,g,b,a);}
	void SetDirection(float x, float y, float z){m_Direction = D3DXVECTOR3(x, y, z);}

	D3DXVECTOR4 GetColour(){return m_Colour;}
	D3DXVECTOR3 GetDirection(){return m_Direction;}
private:
	D3DXVECTOR4 m_Colour;
	D3DXVECTOR3 m_Direction;
};