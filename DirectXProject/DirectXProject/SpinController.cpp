#include "SpinController.h"
#include "GameObject.h"
#include "Orientation.h"

SpinController::SpinController()
{

}


SpinController::~SpinController()
{

}


void SpinController::Update(float time)
{
	Orientation* orientation = &GameObject::GetComponent<Orientation>(GetParentID());
	orientation->Rotate(spinSpeedx*time, D3DXVECTOR3(1,0,0));
	orientation->Rotate(spinSpeedy*time, D3DXVECTOR3(0,1,0));
	orientation->Rotate(spinSpeedz*time, D3DXVECTOR3(0,0,1));
}

void SpinController::SetSpinSpeed(float x, float y, float z)
{
	spinSpeedx = x;
	spinSpeedy = y;
	spinSpeedz = z;
}