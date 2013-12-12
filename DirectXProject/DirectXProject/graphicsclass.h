// Filename: graphicsclass.h
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>


#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "textureshaderclass.h"
#include "lightclass.h"
#include "lightshaderclass.h"
#include "ShaderLibrary.h"
#include "LookupTable.h"
class World;
class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(HWND hwnd, D3DClass* d3d);
	void Shutdown();

	bool Frame(World& world);
private:

	bool Render(World& world);

private:
	D3DClass* m_D3D;
	ShaderLibrary shaderLibrary;

	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
};

#endif
