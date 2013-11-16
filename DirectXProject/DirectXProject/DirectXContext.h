#pragma once

// LINKING //
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// INCLUDES //
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

class Window;

class DirectXContext
{
public:
	DirectXContext(void);
	virtual ~DirectXContext(void);
	virtual void Initialize(Window& window);
	virtual void BeginScene();
	virtual void EndScene();
	ID3D11Device* GetDevice(){return device;}

private:
	bool vsync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
};

