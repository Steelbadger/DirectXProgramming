#include "Application.h"


Application::Application()
{
	m_Input = 0;
	m_Graphics = 0;
}

Application::~Application()
{
}

bool Application::Initialize()
{
	bool result;


	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = &HardwareState::GetInstance();

	window.CreateWindowed("Engine", 800, 800, m_hinstance);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(window.GetWidth(), window.GetHeight(), window.GetHandleToWindow());
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
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Otherwise do the frame processing.
		if (m_Input->Pressed(VK_ESCAPE)) {
			done = true;
		} else {

			result = Frame();
			if(!result)
			{
				done = true;
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

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if(!result)
	{
		return false;
	}

	return true;
}