#pragma once

class World;
class Window;

class Renderer
{
public:
	Renderer(void):world(nullptr){;}
	virtual ~Renderer(void){;}
	virtual void Initialize(Window& window) = 0;
	virtual void Load(World& w) = 0;
	virtual void Render() = 0;

protected:
	World* world;
};

