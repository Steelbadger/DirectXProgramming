#pragma once
#include "Renderer.h"
#include "DirectXContext.h"
#include "LookupTable.h"
#include <map>



class D3DRenderer:public Renderer
{
public:
	D3DRenderer(void);
	virtual ~D3DRenderer(void);
	virtual void Initialize(Window& window);
	virtual void Load(World& w);
	DirectXContext GetContext(){return context;}
	virtual void Render();

	friend class ObjectFactory;

private:
	DirectXContext context;
//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
	HRESULT CompileShaderFromFile( CHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );

	ID3D11VertexShader*                 g_pVertexShader;							//  Renderer, Referenced by Object
	ID3D11PixelShader*                  g_pPixelShader;							//  Renderer, Referenced by Object
	ID3D11InputLayout*                  g_pVertexLayout;							//  Renderer, Referenced by Object
	ID3D11Buffer*                       g_pVertexBuffer;							//  Object, Lookup table in Factory
	ID3D11Buffer*                       g_pIndexBuffer;							//  Object, Lookup table in Factory
	ID3D11Buffer*                       g_pCBChangesEveryFrame;				//  Renderer, via world and camera
	ID3D11ShaderResourceView*           g_pTextureRV;						//  Object, Lookup table in Factory
	ID3D11SamplerState*                 g_pSamplerLinear;						//  Object, Lookup table in Factory
	D3DXMATRIX                          g_World;										//  World
	D3DXMATRIX                          g_View;											//  Fetched from Camera (calculated from quat)
	D3DXMATRIX                          g_Projection;									//  Calculated when needed using camera (fov) and window (ratio)

	World* world;
	Window* window;
};

