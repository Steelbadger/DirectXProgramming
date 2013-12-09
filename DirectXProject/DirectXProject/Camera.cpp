#include "Camera.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"



Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Initialise(bool perspective, float fovDegrees, float viewportWidth, float viewportHeight, float nearPlane, float farPlane)
{
	SetPerspective(perspective);
	SetViewportDimensions(viewportWidth, viewportHeight);
	SetCullPlanes(nearPlane, farPlane);
	SetFieldOfView(fovDegrees);
}

void Camera::SetPerspective(bool mode)
{
	perspective = mode;
	projectionChange = true;
}

void Camera::SetViewportDimensions(float width, float height)
{
	viewportWidth = width;
	viewportHeight = height;
	projectionChange = true;
}

void Camera::SetCullPlanes(float nearPlane, float farPlane)
{
	nearCull = nearPlane;
	farCull = farPlane;
	projectionChange = true;
}

void Camera::SetFieldOfView(float degrees)
{
	fieldOfView = (degrees * (float)D3DX_PI)/180.0f;
	projectionChange = true;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	D3DXVECTOR3 up(0,1,0);
	D3DXVECTOR3 forward(0,0,1);
	D3DXVECTOR3 position = GameObject::GetComponent<Position>(GetParentID()).GetPosition();
	D3DXMATRIX orientationMatrix = GameObject::GetComponent<Orientation>(GetParentID()).GetMatrix();

	D3DXMATRIX output;

	D3DXVec3TransformCoord(&forward, &forward, &orientationMatrix);
	D3DXVec3TransformCoord(&up, &up, &orientationMatrix);

	D3DXVECTOR3 lookAt = position + forward;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&output, &position, &lookAt, &up);

	return output;
}


D3DXMATRIX Camera::GetProjectionMatrix()
{
	if (projectionChange == true) {
		if (perspective) {
			D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, (viewportWidth/viewportHeight), nearCull, farCull);		
		} else {
			D3DXMatrixOrthoLH(&projectionMatrix, viewportWidth, viewportHeight, nearCull, farCull);
		}
		projectionChange = false;
	} 

	return projectionMatrix;
}