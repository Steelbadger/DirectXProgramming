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
	World();
	~World();

	friend class ObjectFactory;

	GameObject& GetObjectByID(ObjectID objectID);
	D3DVECTOR& GetPosition(ObjectID objectID);
	D3DXQUATERNION& GetOrientation(ObjectID objectID);
private:
	DynamicLookupTable<bool> objectUpdateList;
	DynamicLookupTable<GameObject> objects;
	DynamicLookupTable<D3DVECTOR> positions;
	DynamicLookupTable<D3DXQUATERNION> orientations;

};