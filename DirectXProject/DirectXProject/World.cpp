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
#include "DirectionalLight.h"
#include "ShaderLibrary.h"


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
	GameObject::GetComponent<Mesh>(quad).SetMeshData(meshFactory->CreateMeshBuffersFromFile("crate.obj", Mesh::NORMALMAPPED));
	GameObject::GetComponent<Material>(quad).AddTexture<AmbientTexture>("crateDiffuse.jpg");
	GameObject::GetComponent<Material>(quad).AddTexture<NormalMap>("crateNormal.jpg");
	GameObject::GetComponent<Material>(quad).AddTexture<SpecularMap>("crateSpec.jpg");
	GameObject::GetComponent<Material>(quad).SetShader(ShaderLibrary::Shaders::NORMAL);
	GameObject::GetComponent<SpinController>(quad).SetSpinSpeed(0, 0.3f, 0);

	ObjectID test= GameObject::New();
	GameObject::AddComponent<Position>(test);
	GameObject::AddComponent<Orientation>(test);
	GameObject::AddComponent<Mesh>(test);
	GameObject::AddComponent<Material>(test);
	GameObject::AddComponent<SpinController>(test);

	GameObject::GetComponent<Position>(test).SetPosition(0,0,3);
	GameObject::GetComponent<Mesh>(test).SetMeshData(meshFactory->CreateMeshBuffersFromFile("outwardCube.obj", Mesh::NORMALMAPPED));
	GameObject::GetComponent<Material>(test).AddTexture<AmbientTexture>("brick1.dds");
	GameObject::GetComponent<Material>(test).AddTexture<NormalMap>("brick1norm.jpg");
	GameObject::GetComponent<Material>(test).AddTexture<SpecularMap>("brick1spec.jpg");
	GameObject::GetComponent<Material>(test).SetShader(ShaderLibrary::Shaders::NORMAL);
	GameObject::GetComponent<SpinController>(test).SetSpinSpeed(0, 1.0f, 0);


	GameObject::SetParentChild(quad, test);

	ObjectID ground = GameObject::New();
	GameObject::AddComponent<Position>(ground);
	GameObject::AddComponent<Orientation>(ground);
	GameObject::AddComponent<Mesh>(ground);
	GameObject::AddComponent<Material>(ground);

	GameObject::GetComponent<Position>(ground).SetPosition(-25,-0.5,-25);
	GameObject::GetComponent<Mesh>(ground).SetMeshData(meshFactory->CreatePrimitive());
	GameObject::GetComponent<Material>(ground).AddTexture<AmbientTexture>("stone01.dds");
	GameObject::GetComponent<Material>(ground).AddTexture<NormalMap>("bump01.dds");
	GameObject::GetComponent<Material>(ground).AddTexture<SpecularMap>("spec01.jpg");
	GameObject::GetComponent<Material>(ground).SetShader(ShaderLibrary::Shaders::NORMAL);
	AddToScene(ground);



	light = GameObject::New();
	GameObject::AddComponent<DirectionalLight>(light);
	GameObject::AddComponent<SpinController>(light);
	GameObject::GetComponent<DirectionalLight>(light).SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	GameObject::GetComponent<DirectionalLight>(light).SetDirection(1.0f, 0.0f, 0.0f);
	GameObject::GetComponent<DirectionalLight>(light).SetSpecularPower(200.0f);
	GameObject::GetComponent<SpinController>(light).SetSpinSpeed(0.0, 0.0, -0.2f);

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

	if (GameObject::HasComponent<FirstPersonController>(id)) {
		updateList.push_back(id);
	} else {
		Warning("Object has no controller component and was not added to the update list");
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

std::list<ObjectID> World::GetUpdateList()
{
	return updateList;
}

void World::AddToScene(ObjectID id)
{
	if (GameObject::HasComponent<Mesh>(id)) {
		drawList.push_back(id);
	} else {
		Warning("Object has no mesh component and was not added to draw List");
	}

	if (GameObject::HasComponent<SpinController>(id)) {
		updateList.push_back(id);
	} else {
//		Warning("Object has no controller component and was not added to the update list");
	}

	if (GameObject::HasComponent<DirectionalLight>(id)) {
		lightList.push_back(id);
	} else {
//		Warning("Object has no light component and was not added to the light list");
	}
}

void World::PassMeshFactory(MeshFactory* factory)
{
	meshFactory = factory;
}

ObjectID World::GetLight()
{
	return light;
}