#pragma once

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
private:
	bool toBeSynced;
	NetworkManager* network;
};