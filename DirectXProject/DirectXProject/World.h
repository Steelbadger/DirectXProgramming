#pragma once
#include "LookupTable.h"
#include "GameObject.h"
#include "Mesh.h"

typedef struct {
	unsigned int id;
} GameObjectID;

typedef struct {
	unsigned int id;
} MeshID;

class World
{
public:
	World(void);
	~World(void);
	GameObjectID CreateObject();
	MeshID CreateMesh();
	Mesh& GetMesh(MeshID id);
	GameObject& GetObject(GameObjectID id);
	void SetParent(MeshID child, GameObjectID parent);
	void SetParent(GameObjectID child, GameObjectID parent);
	bool HasChanged(){return changed;}
	bool ChangesLoaded(){changed = false;}

private:

	bool changed;
	DynamicLookupTable<GameObject> objects;
	DynamicLookupTable<Mesh> meshes;



};

