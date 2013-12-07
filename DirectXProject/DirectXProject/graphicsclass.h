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


class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, bool vSync, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();
	bool Frame();
	CameraClass* Camera() {return m_Camera;}

private:
	bool Render();
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
};

#endif
