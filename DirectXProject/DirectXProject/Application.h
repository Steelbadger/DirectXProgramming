#pragma once


// PRE-PROCESSING DIRECTIVES //
#define WIN32_LEAN_AND_MEAN

// INCLUDES //
#include <windows.h>
#include <d3d10_1.h>
// MY CLASS INCLUDES //
#include "Hardware.h"
#include "GraphicsWindow.h"
#include "graphicsclass.h"

// Class name: Application
class Application
{
public:
	Application();
	~Application();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPSTR m_applicationName;
	HINSTANCE m_hinstance;

	HardwareState* m_Input;
	GraphicsClass* m_Graphics;
	Window window;
};