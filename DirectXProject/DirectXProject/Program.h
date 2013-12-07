#pragma once
#include "GraphicsWindow.h"

#include <Windows.h>

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
	D3DRenderer renderer;
};