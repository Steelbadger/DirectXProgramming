#pragma once

class ControllerUpdateInterface
{
public:
	ControllerUpdateInterface(){};
	virtual ~ControllerUpdateInterface(){};

	virtual void Update() = 0;
};