#pragma once
#include "LookupTable.h"
//  This is the base class template used for all component classes
//  it provides static storage for all component types 
//  and functions to access these objects based on the ID of the object

template<class T>
class Component
{
public:
	Component(){;}
	~Component(){;}

	ObjectID CopyToStorage(){lookup = componentStorage.Add(*(static_cast<T*>(this)));  return lookup;}
	void DeleteFromStorage(){componentStorage.Remove(lookup);}
	ObjectID GetID(){return lookup;}

	static T& Get(ObjectID id){return componentStorage.Get(id);}
	static void DeleteFromStorage(ObjectID id){componentStorage.Remove(id);}
private:
	ObjectID lookup;
private:
	static DynamicLookupTable<T, 10> componentStorage;
};

template<class T> DynamicLookupTable<T, 10> Component<T>::componentStorage;
