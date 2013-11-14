#include "Hardware.h"
#include "Mouse.h"
#include <windows.h>


HardwareState& HardwareState::GetInstance()
{
	if (instance == NULL) {
		instance = new HardwareState();
	}
	return (*instance);
}

HardwareState* HardwareState::instance;

HardwareState::HardwareState(void)
{
	for (int i = 255; i >= 0; i--)
	{
		oldkey[i] = false;
		keys[i] = false;
		pressed[i] = false;
		released[i] = false;
	}
	timeForLastFrame = 0;
	lastTime = clock();
	currentTime = clock();
	screenWidth =  GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
}


HardwareState::~HardwareState(void)
{
}

void HardwareState::Message(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_KEYDOWN:
			KeyDown(wParam);
			break;
		case WM_KEYUP:
			KeyUp(wParam);
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouse.Message(message, wParam, lParam);
			break;
	}

}

void HardwareState::KeyDown(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=true;
}

void HardwareState::KeyUp(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=false;	
}

void HardwareState::Update()
{
	lastTime = currentTime;
	currentTime = clock();
	timeForLastFrame = currentTime - lastTime;

	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == true && oldkey[i-1] == false)
			pressed[i-1] = true;
		else
			pressed[i-1] = false;
	}
	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == false && oldkey[i-1] == true)
			released[i-1] = true;
		else
			released[i-1] = false;
	}
	memcpy(oldkey, keys, sizeof(keys));
	mouse.Update();
}

double HardwareState::GetTimeSinceLastFrame()
{
	return clock()-currentTime;
}