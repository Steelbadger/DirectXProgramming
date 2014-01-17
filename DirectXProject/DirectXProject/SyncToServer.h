#pragma once
#ifndef _WINSOCK_DEFINED_
#define _WINSOCK_DEFINED_
#include <WinSock2.h>
#endif
#include "Component.h"
#include "GameObject.h"
#include "NetworkManager.h"


class SyncToServer : public Component<SyncToServer>
{
public:
	SyncToServer();
	~SyncToServer();
	void Update();
	void SetObjectToBeSynced();
	static void SetNetworkManager(NetworkManager &manager);
	bool ToBeSynced();
private:
	bool toBeSynced;
	static NetworkManager* network;
	double lastSync;
	static const int UPDATES_PER_SEC = 20;
};