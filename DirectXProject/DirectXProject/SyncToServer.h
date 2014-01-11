#pragma once

#include "Component.h"
#include "GameObject.h"


class SyncToServer : public Component<SyncToServer>
{
public:
	SyncToServer();
	~SyncToServer();
	static void SetServer();
	void Update();
	void SetObjectToBeSynced();
private:
	bool toBeSynced;
};