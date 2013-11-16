#include "D3DRenderer.h"
//#include "World.h"


D3DRenderer::D3DRenderer(void)
{
}


D3DRenderer::~D3DRenderer(void)
{
}

void D3DRenderer::Initialize(Window& window)
{
	context.Initialize(window);
}

//void D3DRenderer::Load(World& w)
//{
//	world = &w;
//}

void D3DRenderer::Render()
{

	//  directx begin draw

	//  for each object in the world

	//  draw it

	//  directx end draw


}

void D3DRenderer::UpdateFromWorld()
{
	//if (world->HasChanged()) {
	//	world->LoadChanges();
	//}
}