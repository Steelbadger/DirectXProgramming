#pragma once
#include "LookupTable.h"
#include "Component.h"
#include <vector>
#include <map>

class GameObject : public Component<GameObject>
{
public:
	GameObject();
	~GameObject();
	ObjectID GetComponentOfType(ComponentType type);
	void AddChild(ObjectID id);
	void RemoveComponent(ComponentType type);
	void RemoveChild(ObjectID id);
	std::vector<ObjectID> GetChildren();

	void AddComponent(ObjectID id, ComponentType type);
	template<class T> void AddComponent(ObjectID id){
		ComponentType type = T::GetComponentTypeID();
		if (components.count(type) == 0) {
			components[type] = id;
			T* tempPointer = &T::Get(id);
			GameObject* checkPointer = this;
			ObjectID thisID = GetID();
			tempPointer->SetParentID(thisID);	
		} else {
			//panic
		} 
	}

	template<class T> ObjectID AddComponent() {
		ComponentType type = T::GetComponentTypeID();		
		ObjectID newComp = T::New(GetID());
		components[type] = newComp;
		return newComp;
	}

	template<class T> void RemoveComponent() {
		ComponentType type = T::GetComponentTypeID();
		ObjectID comp = 0;
		if (components.count(type) != 0) {
			comp = components[type];
			components.erase(type);
			T::DeleteFromStorage(comp);
		} else {
			//panic
		}
	}

	template<class T> T& GetComponent(){
		ComponentType type = T::GetComponentTypeID();
		if(components.count(type) != 0) {
			ObjectID compID = components[type];
			return T::Get(compID);
		} else {
			ObjectID compID = AddComponent<T>();
			return T::Get(compID);
		}
	}

	template<class T> static T& GetComponent(ObjectID id) {
		return GameObject::Get(id).GetComponent<T>();
	}

	template<class T> ObjectID GetComponentReference() {
		ComponentType type = T::GetComponentTypeID();
		ObjectID compID;
		if(components.count(type) != 0) {
			compID = components[type];
		} else {
			compID = AddComponent<T>();
		}
		return compID;
	}

	template<class T> static ObjectID GetComponentReference(ObjectID id) {
		return GameObject::Get(id).GetComponentReference<T>();
	}

private:
	std::map<ComponentType, ObjectID> components;
	std::vector<ObjectID> childGameObjects;		//  array lookup means nothing, just storage
};