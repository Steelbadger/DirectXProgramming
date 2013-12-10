
#include "graphicsclass.h"
#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include "Position.h"
#include "Texture.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Model = 0;
	m_TextureShader = 0;
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, bool vSync, bool fullscreen, float screenDepth, float screenNear)
{
	bool result;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, vSync, hwnd, fullscreen, screenDepth, screenNear);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	//////////////////////////////IMPORTANT////////////////////////////////
	TextureBase::SetDevice(m_D3D->GetDevice());
	///////////////////////////////////////////////////////////////////////

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "brick1.dds");
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

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

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
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

	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool GraphicsClass::TESTFrame()
{
	bool result;

	// Render the graphics scene.
	result = TESTRender();
	if(!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::SetTESTCamera(ObjectID camera)
{
	TESTCamera = camera;
}

bool GraphicsClass::TESTRender()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	static float rotation = 0;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.001f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	viewMatrix = Camera::Get(TESTCamera).GetViewMatrix();
	D3DXMatrixIdentity(&worldMatrix);
	projectionMatrix = Camera::Get(TESTCamera).GetProjectionMatrix();

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());


	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), GameObject::GetComponent<Position>(Camera::Get(TESTCamera).GetParentID()).GetPosition(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

