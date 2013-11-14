#include "GraphicsWindow.h"
#include "Hardware.h"

#include <mmsystem.h>
#include <stdio.h>

std::map<HWND, Window*> Window::WindowMap;

Window::Window(void)
{
}


Window::Window(LPSTR WindowName, int width, int height, HINSTANCE hInstance)
{
	Create(WindowName, width, height, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
}


Window::Window(LPSTR WindowName, int width, int height, HINSTANCE hInstance, bool FullScreen, DWORD Style)
{
	Create(WindowName, width, height, Style, FullScreen, hInstance);
}


Window::~Window(void)
{
	UnregisterClass(wcex.lpszClassName, wcex.hInstance);// Free the window class

	WindowMap.erase(handleToWindow);

	PostQuitMessage(0);		// Post a QUIT message to the window
}

void Window::SetCursorToCentre()
{
	SetCursorPos(gWidth/2+windowRect.left,gHeight/2+windowRect.top);
}

void Window::SetMouseLockedCentre()
{
	HardwareState::GetInstance().SetMouseLocked(gWidth/2,gHeight/2);
}

void Window::OnResize()
{
	GetClientRect(handleToWindow, &graphicsRect);
	GetWindowRect(handleToWindow, &windowRect);

	gHeight = graphicsRect.bottom;
	gWidth = graphicsRect.right;
}

void Window::OnMove()
{
	GetWindowRect(handleToWindow, &windowRect);
}

void Window::Create(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{


	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= &Window::WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= strWindowName;	

	
	RegisterClass(&wcex);// Register the class

	//dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	handleToWindow = CreateWindow(	strWindowName, 
							strWindowName, 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							hInstance,
							NULL
							);

	if(!handleToWindow) {
		MessageBox(NULL, "Could Not Get Handle To Window", "Error", MB_OK); // If we couldn't get a handle, return NULL
		PostQuitMessage (0);
	}
	WindowMap[handleToWindow] = this;

	ShowWindow(handleToWindow, SW_SHOWNORMAL);	
	UpdateWindow(handleToWindow);					
	SetFocus(handleToWindow);

	OnResize();
	titleCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	borderWidth = GetSystemMetrics(SM_CXFRAME);
}

Window* Window::GetWindowReference(HWND hwnd)
{
	return WindowMap[hwnd];
}


LRESULT CALLBACK Window::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CREATE)
    {
		//cxSource = GetSystemMetrics (SM_CXSIZEFRAME) + GetSystemMetrics (SM_CXSIZE);
		//cySource = GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION);
    }
    else
    {
		Window* pWindow = WindowMap[hwnd];
		
        switch (message)
        {
            case WM_PAINT:
                break;
            case WM_COMMAND:
				break;
			case WM_SIZE:
				pWindow->OnResize();
			case WM_MOVE:
				pWindow->OnMove();
            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
            case WM_MOUSEMOVE:
			case WM_KEYDOWN:
			case WM_KEYUP:
				HardwareState::GetInstance().Message(message, wParam, lParam);
				break;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }
    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
