#pragma once

#include <d3d11.h>
#include <vector>
#include <initializer_list>

class RenderTarget
{
public:
	enum Type {AMBIENT = 0, NORMAL = 1, DEPTH = 2, POSITION = 3};

	RenderTarget();
	~RenderTarget();

	bool Initialize(ID3D11Device*, int width, int height, int numTargets);
	bool Initialize(ID3D11Device*, int width, int height, std::initializer_list<Type> targetTypes);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	std::vector<ID3D11ShaderResourceView*> GetShaderResourceViews();

private:
	std::vector<ID3D11Texture2D*> m_targetTextures;
	std::vector<ID3D11RenderTargetView*> m_targetViews;
	std::vector<ID3D11ShaderResourceView*> m_resourceViews;
	static DXGI_FORMAT format[4];
};