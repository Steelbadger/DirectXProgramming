#pragma once

#include "Component.h"
#include "ControllerUpdateInterface.h"


class SpinController : public Component<SpinController>
{
public:
	SpinController();
	~SpinController();

	void Update();
	void SetSpinSpeed(float spinSpeed);
private:
	float spinSpeed;
};