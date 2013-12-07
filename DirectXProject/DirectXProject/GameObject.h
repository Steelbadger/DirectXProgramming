#pragma once
#include "LookupTable.h"
#include <vector>

class GameObject
{
public:
	GameObject();
	~GameObject();
private:
	struct ObjectComponent{
		ObjectID componentID;
		char componentType;
	};
	std::vector<ObjectID> components;
};