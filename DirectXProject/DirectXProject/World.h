#pragma once
#include "LookupTable.h"
#include "Components.h"
#include "MathsClasses.h"

typedef struct {
	unsigned int id;
} GameObjectID;

class World
{
public:
	World(void);
	~World(void);

private:

	struct GameObject {
		unsigned int components[TOTALCOMPONENTS];
	};

	template<typename T>
	struct LookupStorage {
		T object;
		unsigned int parent;
	};


	bool changed;
	DynamicLookupTable< LookupStorage<GameObject> > objects;
	DynamicLookupTable< LookupStorage<Vector3> > positions;
	DynamicLookupTable< LookupStorage<Vector3> > scales;
	DynamicLookupTable< LookupStorage<Quaternion> > orientations;
	DynamicLookupTable< LookupStorage<Mesh> > meshes;
	DynamicLookupTable< LookupStorage<Controller> > controllers;


};

