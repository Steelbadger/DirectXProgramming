#include "SyncToServer.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"
#include <time.h>

NetworkManager* SyncToServer::network;

SyncToServer::SyncToServer()
{
	lastSync = 0;
}

SyncToServer::~SyncToServer()
{

}

void SyncToServer::Update()
{
	double current = clock() - CLOCKS_PER_SEC;
	if (lastSync < clock() - CLOCKS_PER_SEC) {
		lastSync = clock();
		MessageType message;
		message.type = UPDATE;
		message.updateClientID = network->GetID();
		message.xpos = GameObject::GetComponent<Position>(GetParentID()).GetPosition().x;
		message.ypos = GameObject::GetComponent<Position>(GetParentID()).GetPosition().y;
		message.zpos = GameObject::GetComponent<Position>(GetParentID()).GetPosition().z;

		message.xorient = GameObject::GetComponent<Orientation>(GetParentID()).GetOrientation().x;
		message.yorient = GameObject::GetComponent<Orientation>(GetParentID()).GetOrientation().y;
		message.zorient = GameObject::GetComponent<Orientation>(GetParentID()).GetOrientation().z;
		message.s = GameObject::GetComponent<Orientation>(GetParentID()).GetOrientation().w;

	//	std::cout << "Syncing To the Server" << std::endl;

		network->Send(message);
	}
}

void SyncToServer::SetNetworkManager(NetworkManager &manager)
{
	network = &manager;
}

void SyncToServer::SetObjectToBeSynced()
{
	toBeSynced = true;
}

bool SyncToServer::ToBeSynced()
{
	return toBeSynced;
}