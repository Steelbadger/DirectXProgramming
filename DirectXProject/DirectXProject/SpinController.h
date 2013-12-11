#pragma once

#include "Component.h"


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