#include "World.h"


World::World()
{

}


World::~World()
{

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
