#include "DirectXContext.h"
#include "GraphicsWindow.h"

#include <iostream>



DirectXContext::DirectXContext(void)
{
	swapChain = 0;
	device = 0;
	deviceContext = 0;
	renderTargetView = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;
	rasterState = 0;
}


DirectXContext::~DirectXContext(void)
{
}

void DirectXContext::Initialize(Window& window)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;


	// Store the vsync setting.
	vsync_enabled = true;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		std::cerr << "Failed to create graphics interface factor" << std::endl;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		std::cerr << "Failed to create adapter for the primary graphics interface" << std::endl;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		std::cerr << "Failed to Enumerate the primary adapter output" << std::endl;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		std::cerr << "Failed to Get the number of modes that fit the display format for the adapter output" << std::endl;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		std::cerr << "Failed to create a list to hold display modes for this monitor/video card combination" << std::endl;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		std::cerr << "Failed to fill the display mode list structures" << std::endl;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	bool foundMode = false;
	//std::cerr << "Looking For Width: " << window.GetWidth() << "\tHeight: " << window.GetHeight() << std::endl;
	for(i=0; i<numModes; i++)
	{
		//std::cerr << "Mode: " << i << "\tWidth: " << displayModeList[i].Width << "\tHeight: " << displayModeList[i].Height << std::endl;

		if(displayModeList[i].Width == (unsigned int)window.GetWidth())
		{
			if(displayModeList[i].Height == (unsigned int)window.GetHeight())
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				foundMode = true;
			}
		}
	}

	if (!foundMode) {
		std::cerr << "Failed to find a valid display mode" << std::endl;
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		std::cerr << "Failed to get the adapter (video card) description." << std::endl;
	}

	// Store the dedicated video card memory in megabytes.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		std::cerr << "Failed to get the name of the video card" << std::endl;
	}

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = window.GetWidth();
	swapChainDesc.BufferDesc.Height = window.GetHeight();

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = window.GetHandleToWindow();

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(window.IsFullscreen())
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
					       D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the swap chain, Direct3D device, and Direct3D device context." << std::endl;
	}

	// Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		std::cerr << "Failed to get the pointer to the back buffer" << std::endl;
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the render target view with the back buffer pointer." << std::endl;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = window.GetWidth();
	depthBufferDesc.Height = window.GetHeight();
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the texture for the depth buffer." << std::endl;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the depth stencil state." << std::endl;
	}

		// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the depth stencil view." << std::endl;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if(FAILED(result))
	{
		std::cerr << "Failed to create the rasterizer state" << std::endl;
	}

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)window.GetWidth();
	viewport.Height = (float)window.GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);
}

void DirectXContext::BeginScene()
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(renderTargetView, color);
    
	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectXContext::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(vsync_enabled) {
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	} else {
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}
}