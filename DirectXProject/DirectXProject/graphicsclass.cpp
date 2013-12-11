
#include "graphicsclass.h"
#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include "Position.h"
#include "Texture.h"
#include "TextureTypes.h"
#include "MeshFactory.h"
#include "Orientation.h"
#include "Material.h"
#include "World.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_TextureShader = 0;
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(HWND hwnd, D3DClass* d3d)
{
	bool result;

		
	m_D3D = d3d;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the light shader object.", "Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularPower(30.0f);

	return true;
}

void GraphicsClass::Shutdown()
{

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Light)
	{
		delete m_Light;	
		m_Light = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	return;
}


bool GraphicsClass::Frame(World& world)
{
	bool result;

	// Render the graphics scene.
	result = Render(world);
	if(!result)
	{
		return false;
	}

	return true;
}



bool GraphicsClass::Render(World& world)
{
	D3DXMATRIX model;

	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	
	std::list<ObjectID> drawList = world.GetDrawList();

	while (drawList.size() > 0) {
		ObjectID current = drawList.back();
		drawList.pop_back();
		model = GameObject::Get(current).GetLocalMatrix();
		// Render the model using the light shader.	
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), current, model, world.GetCameraObject(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetSpecularPower());
		if(!result)
		{
			return false;
		}
	}
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}



