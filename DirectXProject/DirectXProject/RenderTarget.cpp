#include "RenderTarget.h"


RenderTarget::RenderTarget()
{

}

RenderTarget::~RenderTarget()
{

}



bool RenderTarget::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, int numTargets)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (int i = 0; i < numTargets; i++) {
		// Create the render target texture.
		ID3D11Texture2D* m_renderTargetTexture = 0;
		result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
		if(FAILED(result))
		{
			return false;
		}
		m_targetTextures.push_back(m_renderTargetTexture);
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < numTargets; i++) {

		ID3D11RenderTargetView* m_renderTargetView = 0;
		// Create the render target view.
		result = device->CreateRenderTargetView(m_targetTextures[i], &renderTargetViewDesc, &m_renderTargetView);
		if(FAILED(result))
		{
			return false;
		}
		m_targetViews.push_back(m_renderTargetView);
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < numTargets; i++) {
		ID3D11ShaderResourceView* m_shaderResourceView = 0;
		// Create the shader resource view.
		result = device->CreateShaderResourceView(m_targetTextures[i], &shaderResourceViewDesc, &m_shaderResourceView);
		if(FAILED(result))
		{
			return false;
		}
		m_resourceViews.push_back(m_shaderResourceView);
	}

	return true;
}

void RenderTarget::Shutdown()
{
	while (m_resourceViews.size() > 0) {
		if(m_resourceViews.back())
		{
			m_resourceViews.back()->Release();
			m_resourceViews.pop_back();
		}

		if(m_targetViews.back())
		{
			m_targetViews.back()->Release();
			m_targetViews.pop_back();
		}

		if(m_targetTextures.back())
		{
			m_targetTextures.back()->Release();
			m_targetTextures.pop_back();
		}
	}
	return;
}

void RenderTarget::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(m_targetViews.size(), m_targetViews.data(), depthStencilView);
	
	return;
}

void RenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	for (int i = 0; i < m_targetViews.size(); i++) {
		// Clear the back buffer.
		deviceContext->ClearRenderTargetView(m_targetViews[i], color);
	}
    
	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

std::vector<ID3D11ShaderResourceView*> RenderTarget::GetShaderResourceViews()
{
	return m_resourceViews;
}