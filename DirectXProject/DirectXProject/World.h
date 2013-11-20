#pragma once
#include "LookupTable.h"
#include <D3D10_1.h>
#include <D3DX10math.h>

struct GameObject
{
	ObjectID positionID;
	ObjectID orientationID;
};

class World
{
public:
	World();
	~World();

	friend class ObjectFactory;
	void Initialise();

	GameObject& GetObjectByID(ObjectID objectID);
	D3DVECTOR& GetPosition(ObjectID objectID);
	D3DXQUATERNION& GetOrientation(ObjectID objectID);

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
private:
	DynamicLookupTable<bool> objectUpdateList;
	DynamicLookupTable<GameObject> objects;
	DynamicLookupTable<D3DVECTOR> positions;
	DynamicLookupTable<D3DXQUATERNION> orientations;
};