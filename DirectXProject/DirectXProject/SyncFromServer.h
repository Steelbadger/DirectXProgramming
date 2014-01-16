#pragma once
#ifndef _WINSOCK_DEFINED_
#define _WINSOCK_DEFINED_
#include <WinSock2.h>
#endif
#include "Component.h"
#include "GameObject.h"
#include "NetworkManager.h"


class SyncFromServer : public Component<SyncFromServer>
{
public:
	SyncFromServer();
	~SyncFromServer();
	void Update();
	static void SetNetworkManager(NetworkManager &manager);
	bool MarkedForDelete();
	void SetLinkedClient(unsigned int);
private:
	static NetworkManager* network;
	MessageType previous;
	MessageType secondLast;
	double previousTime;
	double secondLastTime;
	unsigned int linkedClientID;
	bool markForDelete;
};