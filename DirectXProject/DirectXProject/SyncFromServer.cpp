#include "SyncFromServer.h"
#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"
#include "time.h"

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
	double currentTime = clock()/CLOCKS_PER_SEC;
	if (current.timestamp > previous.timestamp) {
		secondLast = previous;
		secondLastTime = previousTime;
		previous = current;
		previousTime = currentTime;
		GameObject::Get(GetParentID()).GetComponent<Position>().SetPosition(current.xpos, current.ypos, current.zpos);
		GameObject::Get(GetParentID()).GetComponent<Orientation>().SetOrientation(D3DXQUATERNION(current.xorient, current.yorient, current.zorient, current.s));
	} else {
		float interpolationFactor = (currentTime-secondLastTime)/(previousTime-secondLastTime);
		D3DXQUATERNION first(secondLast.xorient, secondLast.yorient, secondLast.zorient, secondLast.s);
		D3DXQUATERNION second(previous.xorient, previous.yorient, previous.zorient, previous.s);
		D3DXQUATERNION interp;
		D3DXQuaternionSlerp(&interp, &first, &second, interpolationFactor);
		D3DXVECTOR3 firstp(secondLast.xpos, secondLast.ypos, secondLast.zpos);
		D3DXVECTOR3 secondp(previous.xpos, previous.ypos, previous.zpos);
		D3DXVECTOR3 interpp;
		D3DXVec3Lerp(&interpp, &firstp, &secondp, interpolationFactor);

		GameObject::Get(GetParentID()).GetComponent<Position>().SetPosition(interpp.x, interpp.y, interpp.z);
		GameObject::Get(GetParentID()).GetComponent<Orientation>().SetOrientation(interp);

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