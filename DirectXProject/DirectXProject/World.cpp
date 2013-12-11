#include "World.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Position.h"
#include "Texture.h"
#include "TextureTypes.h"
#include "Position.h"
#include "Orientation.h"
#include "FirstPersonController.h"
#include "UtilityFunctions.h"
#include "Material.h"
#include "SpinController.h"


World::World()
{
}


World::~World()
{

}


void World::CreateScene()
{
	ObjectID quad = GameObject::New();
	GameObject::AddComponent<Position>(quad);
	GameObject::AddComponent<Orientation>(quad);
	GameObject::AddComponent<Mesh>(quad);
	GameObject::AddComponent<Material>(quad);
	GameObject::AddComponent<SpinController>(quad);

	GameObject::GetComponent<Position>(quad).SetPosition(0,0,0);
	GameObject::GetComponent<Mesh>(quad).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::LIT));
	GameObject::GetComponent<Material>(quad).AddTexture<AmbientTexture>("brick1.dds");
	GameObject::GetComponent<SpinController>(quad).SetSpinSpeed(0.04f);

	ObjectID test= GameObject::New();
	GameObject::AddComponent<Position>(test);
	GameObject::AddComponent<Orientation>(test);
	GameObject::AddComponent<Mesh>(test);
	GameObject::AddComponent<Material>(test);
	GameObject::AddComponent<SpinController>(test);

	GameObject::GetComponent<Position>(test).SetPosition(0,0,3);
	GameObject::GetComponent<Mesh>(test).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::LIT));
	GameObject::GetComponent<Material>(test).AddTexture<AmbientTexture>("seafloor.dds");
	GameObject::GetComponent<SpinController>(test).SetSpinSpeed(0.2f);

	GameObject::SetParentChild(quad, test);

	AddToScene(quad);
	AddToScene(test);
}

void World::SetCameraObject(ObjectID id)
{
	if (GameObject::HasComponent<Camera>(id)) {
		currentCamera = id;
	} else {
		Warning("Object has no camera component and was not added to world");
	}
}

ObjectID World::GetCameraObject()
{
	return currentCamera;
}

std::list<ObjectID> World::GetDrawList()
{
	return drawList;
}

void World::AddToScene(ObjectID id)
{
	if (GameObject::HasComponent<Mesh>(id)) {
		drawList.push_back(id);
	} else {
		Warning("Object has no mesh component and was not added to draw List");
	}
}

void World::PassMeshFactory(MeshFactory* factory)
{
	meshFactory = factory;
}