#include "World.h"


World::World()
{

}


World::~World()
{

}

void World::Initialise()
{
	// Setup the projection matrix.
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)1440 / (float)900;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, 0.1, 1000);

    // Initialize the world matrix to the identity matrix.
    D3DXMatrixIdentity(&worldMatrix);

	D3DXVECTOR3 up;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = -10.0f;
	position.y = 6.0f;
	position.z = -10.0f;

	// Calculate the rotation in radians.
	radians = 45.0f * 0.0174532925f;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + -10.0f;
	lookAt.y = 6.0f;
	lookAt.z = cosf(radians) + -10.0f;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

}

GameObject& World::GetObjectByID(ObjectID objectID)
{
	return objects.Get(objectID);
}

D3DVECTOR& World::GetPosition(ObjectID objectID)
{
	return positions.Get(objects.Get(objectID).positionID);
}

D3DXQUATERNION& World::GetOrientation(ObjectID objectID)
{
	return orientations.Get(objects.Get(objectID).orientationID);
}
