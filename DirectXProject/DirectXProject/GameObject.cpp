#include "GameObject.h"


const Vector4 GameObject::GLOBALX = Vector4(1.0, 0, 0, 0);
const Vector4 GameObject::GLOBALY = Vector4(0, 1.0, 0, 0);
const Vector4 GameObject::GLOBALZ = Vector4(0, 0, 1.0, 0);

GameObject::GameObject(void):
	parent(nullptr),
		objectType(ABSTRACT),
		position(0,0,0),
		rotation(0, 0, 0, 0)
{
}

GameObject::GameObject(ObjectType t):
	parent(nullptr),
		objectType(t),
		position(0,0,0),
		rotation(0,0,0,0)
{
}


GameObject::~GameObject(void)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		(*it)->ClearParent();
		children.erase(it);
	}
}

bool GameObject::HasParent() {
	return ((parent == nullptr) ? false : true);
}

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::SetParent(GameObject& g)
{
	parent = &g;
	g.AddChild(*this);
}

void GameObject::AddChild(GameObject& g)
{
	children.push_back(&g);
}

void GameObject::ClearParent()
{
	parent = nullptr;
}

bool GameObject::RemoveChild(GameObject& g)
{
	std::vector<GameObject*>::iterator it;
	for (it = children.begin(); it != children.end(); it++) {
		if (*it == &g) {
			g.ClearParent();
			children.erase(it);
			return true;
		}
	}
	return false;
}

GameObject::ObjectType GameObject::GetType()
{
	return objectType;
}

void GameObject::SetPosition(Vector3 p)
{
	position = p;
}

void GameObject::SetRotation(Quaternion r)
{
	rotation = r;
}

Vector4 GameObject::GetPosition()
{
	return position;
}

Quaternion GameObject::GetRotation()
{
	return rotation;
}
Matrix4x4 GameObject::GetRotationMatrix()
{
	return rotation.GetRotationMatrix();
}

Vector4 GameObject::GetLocalX()
{
	Quaternion localX = (rotation * Quaternion(GLOBALX)) * rotation.Inverse();

	return localX.Normalize();
}
Vector4 GameObject::GetLocalY()
{
	Quaternion localY = (rotation * Quaternion(GLOBALY)) * rotation.Inverse();

	return localY.Normalize();
}

Vector4 GameObject::GetLocalZ()
{
	Quaternion localZ = (rotation * Quaternion(GLOBALZ)) * rotation.Inverse();

	return localZ.Normalize();
}