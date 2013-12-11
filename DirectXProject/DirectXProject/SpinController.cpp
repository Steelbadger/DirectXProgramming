#include "SpinController.h"
#include "GameObject.h"
#include "Orientation.h"

SpinController::SpinController()
{

}


SpinController::~SpinController()
{

}


void SpinController::Update()
{
	Orientation* orientation = &GameObject::GetComponent<Orientation>(GetParentID());
	orientation->Rotate(spinSpeed, D3DXVECTOR3(0,1,0));
}

void SpinController::SetSpinSpeed(float s)
{
	spinSpeed = s;
}