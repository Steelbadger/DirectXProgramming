#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

struct MatrixBufferType
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

const D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
	{ "NORMAL",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "MODELMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//	{ "MODELMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//	{ "MODELMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
//	{ "MODELMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
};

//const UINT LAYOUTELEMENTS = 7;
const UINT LAYOUTELEMENTS = 3;

struct RenderData
{
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;

	int stride;
	int indexCount, vertexCount;

	ID3D11ShaderResourceView** textures;
	int numTextures;

	ID3D11InputLayout* vertLayout;

	unsigned int objectID;

	D3DVECTOR* position;
	D3DXQUATERNION* orientation;
};