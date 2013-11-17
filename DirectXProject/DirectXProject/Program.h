#pragma once
#include "GraphicsWindow.h"

#include <Windows.h>

#include "World.h"
#include "ObjectFactory.h"
#include "D3DRenderer.h"

class HardwareState;
class DirectXContext;

class Program
{
public:
	Program(void);
	~Program(void);

	void Initialize(HINSTANCE hInstance);
	void MainLoop();
private:
	Window window;
	HardwareState* hardware;
	DirectXContext* context;
	World world;
	ObjectFactory factory;
	D3DRenderer renderer;
};