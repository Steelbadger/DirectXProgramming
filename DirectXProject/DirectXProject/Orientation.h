#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class Orientation : public Component<Orientation>
{
public:
	Orientation(){D3DXQuaternionIdentity(&orientation);}
	~Orientation(){;}

	void SetOrientation(D3DXQUATERNION in){orientation = in;}
	void SetOrientation(D3DXMATRIX in){D3DXQuaternionRotationMatrix(&orientation, &in);}
	void Rotate(float angle, D3DXVECTOR3 axis){D3DXQuaternionRotationAxis(&orientation, &axis, angle);}
	D3DXMATRIX GetMatrix(){D3DXMATRIX out;  D3DXMatrixRotationQuaternion(&out, &orientation); return out;}
private:
	D3DXQUATERNION orientation;
};