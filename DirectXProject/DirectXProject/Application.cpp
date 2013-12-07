#include "Application.h"

#include "Orientation.h"


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
	Position position;
	Orientation orient;

	positionComp = position.CopyToStorage();
	ObjectID otherComp = position.CopyToStorage();
	ObjectID orientID = orient.CopyToStorage();


	Position::Get(positionComp).SetPosition(10.0f, 10.0f, 10.0f);
	Position::Get(positionComp).Move(2.0f, 0.0f, -3.0f);
	Component<Position>::Get(positionComp).Move(3.0f, 2.0f, -5.0f);

	Orientation::Get(orientID).Rotate(34.0f, D3DXVECTOR3(1, 0, 0));

	D3DXVECTOR3 vec = Position::Get(positionComp).GetPosition();
	char poscompid = Position::GetComponentTypeID();
	char orientCompID = Orientation::GetComponentTypeID();
	char totNumComps = ComponentBase::GetNumberOfComponents();

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = &HardwareState::GetInstance();


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
	
	return true;
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
			result = Frame();
			if(!result)
			{
				running = false;
			}
		}
		
	}

	return;

}

bool Application::Frame()
{
	bool result;

	if (m_Input->Button(VK_UP)) {
		m_Graphics->Camera()->SetPosition(m_Graphics->Camera()->GetPosition().x, m_Graphics->Camera()->GetPosition().y, m_Graphics->Camera()->GetPosition().z+0.025f);
	}

	if (m_Input->Button(VK_DOWN)) {
		m_Graphics->Camera()->SetPosition(m_Graphics->Camera()->GetPosition().x, m_Graphics->Camera()->GetPosition().y, m_Graphics->Camera()->GetPosition().z-0.025f);
	}

	if (m_Input->Button(VK_LEFT)) {
		m_Graphics->Camera()->SetPosition(m_Graphics->Camera()->GetPosition().x+0.025f, m_Graphics->Camera()->GetPosition().y, m_Graphics->Camera()->GetPosition().z);
	}

	if (m_Input->Button(VK_RIGHT)) {
		m_Graphics->Camera()->SetPosition(m_Graphics->Camera()->GetPosition().x-0.025f, m_Graphics->Camera()->GetPosition().y, m_Graphics->Camera()->GetPosition().z);
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
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