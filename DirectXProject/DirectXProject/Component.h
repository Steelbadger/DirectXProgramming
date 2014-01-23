#pragma once
#include "LookupTable.h"
//  This describes the basis for the component system

typedef unsigned char ComponentType;

//////////////////////////////////////////////////////////
//    This is the base class for all components, it's	//
//     one purpose is to track the number of classes	//
//     inheriting from it, this can then be used to     //
//   keep track of which component is which in arrays   //
//                of just component IDs					//
//////////////////////////////////////////////////////////
class ComponentBase
{
public:
	ComponentBase(){;}
	~ComponentBase(){;}
	static ComponentType GetNumberOfComponents(){return componentCount;}
	static LookupTableInterface* GetComponentStorage(ComponentType componentTypeID) {return (storageInterfaces.size() >= componentTypeID ? storageInterfaces[componentTypeID] : NULL);}
protected:
	static void SetComponentStorage(ComponentType componentTypeID, LookupTableInterface* dataInterface){storageInterfaces[componentTypeID] = dataInterface;}
	static ComponentType NewComponentType(LookupTableInterface* data) {
		ComponentType output = componentCount++;
		storageInterfaces.push_back(data);
		return output;
	}
	static ComponentType componentCount;
	static std::vector<LookupTableInterface*> storageInterfaces;
};



//////////////////////////////////////////////////////////
//  This class is the meat of the component system, it  //
//    updates the ComponentBase class each time a new   //
//      variant is created.  It handles storage and     //
//     referencing of each component object within a    //
//   resizable array lookup table.  All new components  //
//      should inherit from this class in the form      //
//  class NewComponentType :							//
//					public Component<NewComponentType>	//
//////////////////////////////////////////////////////////
template<class T>
class Component : protected ComponentBase
{
public:
	~Component(){;}

	ObjectID CopyToStorage(){
		lookup = componentStorage.Add(*(static_cast<T*>(this)));
		componentStorage.Get(lookup).SetLookup(lookup);
		return lookup;
	}

	void DeleteFromStorage(){componentStorage.Remove(lookup);}
	ObjectID GetID(){return lookup;}
	bool HasParent(){return ((parent > -1) ? true : false);}
	ObjectID GetParentID(){return parent;}
	void SetParentID(ObjectID id){
		parent = id;

	}

	void SetLookup(ObjectID id) {
		lookup = id;
	}

	static ComponentType GetComponentTypeID(){return componentTypeID;}
	static T& Get(ObjectID id){return componentStorage.Get(id);}
	static void DeleteFromStorage(ObjectID id){componentStorage.Remove(id);}
	static ObjectID New(){return T().CopyToStorage();}
	static ObjectID New(ObjectID p) {
		ObjectID newItem = T().CopyToStorage();
		Get(newItem).SetParentID(p);
		return newItem;
	}
	static DynamicLookupTable<T>& GetList(){
		return componentStorage;
	}
protected:
	Component(): lookup(-1), parent(-1){
		if (componentClassCreated == false) {
//			componentTypeID = componentCount++;
			ComponentBase herp;
			componentClassCreated = true;
			componentTypeID = NewComponentType((LookupTableInterface*)(&componentStorage));
//			SetComponentStorage(componentTypeID, (LookupTableInterface*)&componentStorage);
		}
	}
private:
	ObjectID lookup;
	ObjectID parent;
private:
	static DynamicLookupTable<T> componentStorage;
	static bool componentClassCreated;
	static ComponentType componentTypeID;
};

template<class T> DynamicLookupTable<T, 10> Component<T>::componentStorage;
template<class T> bool Component<T>::componentClassCreated = false;
template<class T> ComponentType Component<T>::componentTypeID;
