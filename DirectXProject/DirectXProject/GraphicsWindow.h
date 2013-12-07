#pragma once
#include <Windows.h>
#include <map>

class Application;

class Window
{
public:
	Window(Application* parent);
	void Create(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
	void CreateFullScreen(LPSTR strWindowName, HINSTANCE hInstance);
	void CreateWindowed(LPSTR strWindowName, int width, int height, HINSTANCE hInstance);

	~Window(void);

	void OnResize();
	void OnMove();

	int GetWidth(){return graphicsRect.right;}
	int GetHeight(){return graphicsRect.bottom;}
	int GetGraphicsYOffset(){return borderWidth+titleCaptionHeight;}
	int GetGraphicsXOffset(){return borderWidth;}
	int GetX(){return windowRect.left;};
	int GetY(){return windowRect.top;}

	void SetCursorToCentre();
	void SetMouseLockedCentre();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static Window* GetWindowReference(HWND hWnd);

	HWND GetHandleToWindow(){return handleToWindow;}
private:

	HWND handleToWindow;
	WNDCLASS wcex;
	RECT graphicsRect;
	RECT windowRect;

	Application* parent;

	int gWidth, gHeight;
	int borderWidth;
	int titleCaptionHeight;
	static std::map<HWND,Window*>WindowMap;



	static const int COLOUR_DEPTH = 16;
};

