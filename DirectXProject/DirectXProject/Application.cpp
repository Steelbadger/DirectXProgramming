#include "Application.h"

#include "Orientation.h"
#include "GameObject.h"
#include "Camera.h"


Application::Application(): window(this)
{
	m_Input = 0;
	m_Graphics = 0;
	fullscreen = true;
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

	//  Can create components within their  static storage using New()
	positionComp = Position::New();
	ObjectID orientID = Orientation::New();
	ObjectID mainObj = GameObject::New();

	//  Can add components to an object using two different methods, using either the component type unique id
	GameObject::Get(mainObj).AddComponent(orientID, Component<Orientation>::GetComponentTypeID());
	//  Or templated types
	GameObject::Get(mainObj).AddComponent<Position>(positionComp);

	//  Retrieve the object held in static storage using the ID of the object, this can be used to access the
	//  methods of the component
	Position::Get(positionComp).SetPosition(10.0f, 10.0f, 10.0f);
	Position::Get(positionComp).Move(2.0f, 0.0f, -3.0f);

	//  This can be done using either the derived class, or the templated base class
	Component<Position>::Get(positionComp).Move(3.0f, 2.0f, -5.0f);


	Orientation::Get(orientID).Rotate(34.0f, D3DXVECTOR3(1, 0, 0));


	//  Get the reference of the component object for use in maths and logic
	D3DXVECTOR3 vec = Position::Get(positionComp).GetPosition();

	//  Get the child component of GameObject of the specified type (using unique ID of component type)
	D3DXVECTOR3 vec2 = Position::Get(GameObject::Get(mainObj).GetComponentOfType(Position::GetComponentTypeID())).GetPosition();


	//  This is the more heavily templated way, also easier to use.
	//  Create new GameObjects using New()
	ObjectID secondaryObject = GameObject::New();

	//  Add components using templated functions, these functions only accept types inheriting from the Component class
	GameObject::Get(secondaryObject).AddComponent<Position>();
	GameObject::Get(secondaryObject).AddComponent<Orientation>();

	//  retrieve the member components using templated getters, can then directly modify the member data using class methods
	GameObject::Get(secondaryObject).GetComponent<Position>().SetPosition(5.0f, 12.0f, 17.0f);
	GameObject::Get(secondaryObject).GetComponent<Orientation>().SetOrientation(D3DXQUATERNION(34, 0, 1, 0));

	//  Static function for the same purpose, shortens the function call.
	GameObject::GetComponent<Position>(mainObj);

	//  Store references to the component in question gy getting using the templated functions
	GameObject* pointy = &GameObject::Get(secondaryObject);
	Position* posPointy = &pointy->GetComponent<Position>();
	Orientation* orientPointy = &pointy->GetComponent<Orientation>();


	//  Functions to fetch the unique id of a given component type
	char poscompid = Position::GetComponentTypeID();
	char orientCompID = Orientation::GetComponentTypeID();

	//  Can also count the number of components types currently in existance.
	char totNumComps = ComponentBase::GetNumberOfComponents();


	////////  CAMERA TEST /////////
	//  Create the Camera Object with position, orientation and a camera component
	ObjectID cam = GameObject::New();
	GameObject::Get(cam).AddComponent<Position>();
	GameObject::Get(cam).AddComponent<Orientation>();
	GameObject::Get(cam).AddComponent<Camera>();

	//  Set to the default position
	GameObject::GetComponent<Position>(cam).SetPosition(0,0,-5.0);
	GameObject::GetComponent<Camera>(cam).Initialise(true, 45, window.GetWidth(), window.GetHeight(), 0.1f, 1000.0f);

	//  Add to the GraphicsClass
	m_Graphics->SetTESTCamera(GameObject::GetComponentReference<Camera>(cam));
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
//	result = m_Graphics->Frame();

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