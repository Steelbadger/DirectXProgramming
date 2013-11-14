#pragma once
#include "GraphicsWindow.h"

#include <Windows.h>


class HardwareState;
class GraphicsContext;

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
	GraphicsContext* context;
};