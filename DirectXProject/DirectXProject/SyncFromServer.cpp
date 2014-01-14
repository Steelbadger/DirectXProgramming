#include "SyncFromServer.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"

NetworkManager* SyncFromServer::network;

SyncFromServer::SyncFromServer()
{
	previous.timestamp = 0;
}

SyncFromServer::~SyncFromServer()
{

}

void SyncFromServer::Update()
{
	MessageType current = network->GetLastUpdate(linkedClientID);
	if (current.timestamp > previous.timestamp) {
		secondLast = previous;
		previous = current;
		GameObject::Get(GetParentID()).GetComponent<Position>().SetPosition(current.xpos, current.ypos, current.zpos);
		GameObject::Get(GetParentID()).GetComponent<Orientation>().SetOrientation(D3DXQUATERNION(current.s, current.xorient, current.yorient, current.zorient));
	}
}

void SyncFromServer::SetNetworkManager(NetworkManager &manager)
{
	network = &manager;
}

void SyncFromServer::SetLinkedClient(unsigned int client)
{
	linkedClientID = client;
}