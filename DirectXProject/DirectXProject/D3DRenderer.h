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
//	virtual void Load(World& w);
	virtual void Render();

private:
	void UpdateFromWorld();
	DirectXContext context;

	struct MeshBuffer {
		ID3D11Buffer *vertexBuffer, *indexBuffer;
	};

	struct MaterialBuffer {
		ID3D11ShaderResourceView* m_texture;
	};

	DynamicLookupTable<float> derp;
	
};

