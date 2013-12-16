#pragma once

#include <d3d11.h>
#include <vector>
#include <initializer_list>

class RenderTarget
{
public:

	RenderTarget();
	~RenderTarget();

	bool Initialize(ID3D11Device*, int width, int height, int numTargets);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	std::vector<ID3D11ShaderResourceView*> GetShaderResourceViews();

private:
	std::vector<ID3D11Texture2D*> m_targetTextures;
	std::vector<ID3D11RenderTargetView*> m_targetViews;
	std::vector<ID3D11ShaderResourceView*> m_resourceViews;
};