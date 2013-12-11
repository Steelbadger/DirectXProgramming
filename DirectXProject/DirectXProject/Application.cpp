#include "Application.h"

#include "Orientation.h"
#include "GameObject.h"
#include "Camera.h"
#include "FirstPersonController.h"
#include "Material.h"


#include <functional>

Application::Application(): window(this)
{
	m_Input = 0;
	m_Graphics = 0;
	fullscreen = false;
	vSyncEnabled = true;
}

Application::~Application()
{
}

bool Application::Initialize()
{
	bool result;
	running = true;

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = &HardwareState::GetInstance();

	//  Before creating the Windows window, tell the container what Message Handler function to use.
	window.SetMessageHandler<Application>((*this), std::mem_fn(&Application::MessageHandler));


	if (fullscreen)
		window.CreateFullScreen("Engine", m_hinstance);
	else
		window.CreateWindowed("Engine", 800, 800, m_hinstance);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(window.GetWidth(), window.GetHeight(), window.GetHandleToWindow(), vSyncEnabled, fullscreen, 1000.0f, 0.1f);
	if(!result)
	{
		return false;
	}

	TestFunction();
	
	return true;
}

void Application::TestFunction()
{
	////////  CAMERA TEST /////////
	//  Create the Camera Object with position, orientation and a camera component
	ObjectID cam = GameObject::New();
	GameObject::Get(cam).AddComponent<Position>();
	GameObject::Get(cam).AddComponent<Orientation>();
	GameObject::Get(cam).AddComponent<Camera>();

	//  Oh, and a controller
	GameObject::Get(cam).AddComponent<FirstPersonController>();

	//  Set to the default position, initialise camera and set control sensitivity
	GameObject::GetComponent<Position>(cam).SetPosition(0,0,-5.0);
	GameObject::GetComponent<Camera>(cam).Initialise(true, 45, window.GetWidth(), window.GetHeight(), 0.1f, 1000.0f);
	GameObject::GetComponent<FirstPersonController>(cam).SetSensitivity(20.0f);

	Camera* camSecond = &GameObject::GetComponent<Camera>(cam);

	//  Add to the GraphicsClass
	m_Graphics->SetTESTCamera(GameObject::GetComponentReference<Camera>(cam));

	m_Graphics->TESTinit();
}

void Application::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	return;
}

void Application::Run()
{
	MSG msg;
	bool result;
	m_Input->Update();


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	while(running)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Otherwise do the frame processing.
		if (m_Input->Pressed(VK_ESCAPE)) {
			running = false;
		} else {
			m_Input->Update();
			TestUpdate();
			result = Frame();
			if(!result)
			{
				running = false;
			}
		}
		
	}

	return;

}

void Application::TestUpdate()
{
	for (int i = 0; i < FirstPersonController::GetList().Size(); i++) {
		if (FirstPersonController::GetList().Exists(i)) {
			FirstPersonController::GetList().Get(i).Update();
		}
	}
	if (m_Input->MouseButton(Mouse::RIGHT)) {
		window.SetCursorToCentre();
		window.SetMouseLockedCentre();
	}
}

bool Application::Frame()
{
	bool result;

	result = m_Graphics->TESTFrame();
	if(!result)
	{
		return false;
	}

	return true;
}

void Application::MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)
        {
			case WM_SIZE:
				window->OnResize();
			case WM_MOVE:
				window->OnMove();
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
				running = false;
                PostQuitMessage(0);
				break;
        }
}