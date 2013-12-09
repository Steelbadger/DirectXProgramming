#pragma once

#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class Camera : public Component<Camera>
{
public:
	Camera();
	~Camera();
	void Initialise(bool perspective, float fovDegrees, float viewportWidth, float viewportHeight, float nearPlane, float farPlane);
	void SetPerspective(bool p);
	void SetViewportDimensions(float width, float height);
	void SetCullPlanes(float nearPlane, float farPlane);
	void SetFieldOfView(float degrees);
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjectionMatrix();

private:
	bool perspective;
	float fieldOfView, nearCull, farCull, viewportWidth, viewportHeight;
	D3DXMATRIX projectionMatrix;
	bool projectionChange;
};