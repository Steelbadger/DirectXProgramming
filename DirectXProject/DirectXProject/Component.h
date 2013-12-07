#pragma once
#include "LookupTable.h"
//  This describes the basis for the component system



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
	static char GetNumberOfComponents(){return componentCount;}
protected:
	static char componentCount;
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
	Component(){
		if (componentClassCreated == false) {
			componentTypeID = componentCount++;	
			componentClassCreated = true;
		}
	}
	~Component(){;}

	ObjectID CopyToStorage(){lookup = componentStorage.Add(*(static_cast<T*>(this)));  return lookup;}
	void DeleteFromStorage(){componentStorage.Remove(lookup);}
	ObjectID GetID(){return lookup;}

	static char GetComponentTypeID(){return componentTypeID;}
	static T& Get(ObjectID id){return componentStorage.Get(id);}
	static void DeleteFromStorage(ObjectID id){componentStorage.Remove(id);}
private:
	ObjectID lookup;
private:
	static DynamicLookupTable<T, 10> componentStorage;
	static bool componentClassCreated;
	static char componentTypeID;
};

template<class T> DynamicLookupTable<T, 10> Component<T>::componentStorage;
template<class T> bool Component<T>::componentClassCreated = false;
template<class T> char Component<T>::componentTypeID;
