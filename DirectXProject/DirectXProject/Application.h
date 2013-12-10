#pragma once


// PRE-PROCESSING DIRECTIVES //
#define WIN32_LEAN_AND_MEAN

// INCLUDES //
#include <windows.h>
// MY CLASS INCLUDES //
#include "Hardware.h"
#include "GraphicsWindow.h"
#include "graphicsclass.h"
#include "Position.h"

// Class name: Application
class Application
{
public:
	Application();
	~Application();

	bool Initialize();
	void Shutdown();
	void Run();

	void MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void TestFunction();
	void TestUpdate();

private:
	LPSTR m_applicationName;
	HINSTANCE m_hinstance;

	HardwareState* m_Input;
	GraphicsClass* m_Graphics;
	Window window;

	bool fullscreen, vSyncEnabled;

	ObjectID positionComp;

	bool running;
};