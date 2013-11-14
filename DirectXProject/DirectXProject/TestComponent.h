#pragma once

class Component
{
public:
	const static int componentCounter = 0;
};


template<int n = 0>
class ComponentExtender : public Component
{
public:
	const static int componentCounter = &Component::componentCounter;	
};

class NewComponent : public ComponentExtender<Component::componentCounter+1>
{
	NewComponent(){;}
	~NewComponent(){;}
	void Herp(){;}
}