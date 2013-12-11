#include "GameObject.h"

#include "Position.h"
#include "Orientation.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

ObjectID GameObject::GetComponentOfType(ComponentType type)
{
	return components[type];
}

D3DXMATRIX GameObject::GetLocalMatrix()
{
	D3DXMATRIX output;
	D3DXMatrixIdentity(&output);
	D3DXMATRIX position;
	D3DXMatrixIdentity(&position);
	D3DXMATRIX orientation;
	D3DXMatrixIdentity(&orientation);
	if (GameObject::HasComponent<Position>(GetParentID())) {
		D3DXVECTOR3 pos = GameObject::GetComponent<Position>(GetParentID()).GetPosition();
		D3DXMatrixTranslation(&position, pos.x, pos.y, pos.z);
	}
	if (GameObject::HasComponent<Orientation>(GetParentID())) {
		orientation = GameObject::GetComponent<Orientation>(GetParentID()).GetMatrix();
	}



}

void GameObject::AddChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			Error("Duplicate child, not added");
			//duplicate, panic!
			return;
		}
	}
	childGameObjects.push_back(id);
	GameObject::Get(id).SetParentID(GetID());
}

void GameObject::AddComponent(ObjectID id, ComponentType type)
{
	if (components.count(type) == 0) {
		components[type] = id;
	} else {
		Error("Duplicate component, not added");
		//panic
	} 
}

void GameObject::RemoveComponent(ComponentType type)
{
	if (components.count(type) != 0) {
		components.erase(type);
	} else {
		Error("Cannot remove component, component does not exist");
		//panic
	}
}

void GameObject::RemoveChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			childGameObjects.erase(it);
			return;
		}
	}
	Error("Specified object is not a child of this object");
	//panic
}

std::vector<ObjectID> GameObject::GetChildren()
{
	return childGameObjects;
}