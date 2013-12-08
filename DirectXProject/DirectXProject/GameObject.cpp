#include "GameObject.h"


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

void GameObject::AddChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			//duplicate, panic!
			return;
		}
	}
	childGameObjects.push_back(id);
}

void GameObject::AddComponent(ObjectID id, ComponentType type)
{
	if (components.count(type) == 0) {
		components[type] = id;
	} else {
		//panic
	} 
}

void GameObject::RemoveComponent(ComponentType type)
{
	if (components.count(type) != 0) {
		components.erase(type);
	} else {
		//panic
	}
}

void GameObject::RemoveChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			childGameObjects.erase(it);
		}
	}
	//panic
}

std::vector<ObjectID> GameObject::GetChildren()
{
	return childGameObjects;
}