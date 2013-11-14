#pragma once
#include "MathsClasses.h"
#include <vector>

class GameObject
{
public:
	static const Vector4 GLOBALX, GLOBALY, GLOBALZ;

	enum ObjectType {ABSTRACT, O3D, O2D}; 

	GameObject(void);
	GameObject(ObjectType t);
	~GameObject(void);
	bool HasParent();
	GameObject* GetParent();
	void SetParent(GameObject& g);
	void ClearParent();
	ObjectType GetType();
	void SetPosition(Vector3 position);
	void SetRotation(Quaternion rotation);
	void AddChild(GameObject& g);
	bool RemoveChild(GameObject& g);

	//  Getters
	Vector3 GetScale(){return scale;}
	Vector4 GetPosition();
	Vector4 GetLocalX();
	Vector4 GetLocalY();
	Vector4 GetLocalZ();
	Quaternion GetRotation();
	Matrix4x4 GetRotationMatrix();

private:
	GameObject* parent;
	ObjectType objectType;

	std::vector<GameObject*> children;

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
};

