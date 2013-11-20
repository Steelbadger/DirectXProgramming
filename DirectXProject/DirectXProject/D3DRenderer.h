#pragma once
#include "Renderer.h"
#include "DirectXContext.h"
#include "LookupTable.h"
#include "RenderData.h"
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
	DynamicLookupTable<RenderData> data;
	DirectXContext context;

	World* world;

	void RenderObject(RenderData objectToRender);
};

