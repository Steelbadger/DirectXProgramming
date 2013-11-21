#include "D3DRenderer.h"
#include "World.h"
//#include "UtilityFunctions.h"
#include <iostream>


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

void D3DRenderer::Load(World& w)
{
	world = &w;
}

void D3DRenderer::Render()
{

	//  directx begin draw

	for (int i = 0; i < data.Size(); i++) {
		std::cout << "Draw " << i << std::endl;
		if (data.Exists(i)) {
			RenderObject(data.Get(i));
		}
	}
	//  for each object in the world

	//  draw it

	//  directx end draw
}

void D3DRenderer::RenderObject(RenderData objectToRender)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	D3DXMATRIX worl;
	D3DXMATRIX view;
	D3DXMATRIX proj;


	// Set shader texture resource in the pixel shader.
	context.GetDeviceContext()->PSSetShaderResources(0, objectToRender.numTextures, &objectToRender.textures);

	// Lock the matrix constant buffer so it can be written to.
	result = context.GetDeviceContext()->Map(objectToRender.matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		std::cerr << "Failed to Map Matrix Buffer" << std::endl;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worl, &world->worldMatrix);
	D3DXMatrixTranspose(&view, &world->viewMatrix);
	D3DXMatrixTranspose(&proj, &world->projectionMatrix);

	// Copy the matrices into the constant buffer.
	dataPtr->world = worl;
	dataPtr->view = view;
	dataPtr->projection = proj;

	// Unlock the matrix constant buffer.
    context.GetDeviceContext()->Unmap(objectToRender.matrixBuffer, 0);

	// Set the position of the matrix constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the matrix constant buffer in the vertex shader with the updated values.
    context.GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &objectToRender.matrixBuffer);

	// Lock the light constant buffer so it can be written to.
	//result = context.GetDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if(FAILED(result))
	//{
	//	return false;
	//}

	//// Get a pointer to the data in the constant buffer.
	//dataPtr2 = (LightBufferType*)mappedResource.pData;

	//// Copy the lighting variables into the constant buffer.
	//dataPtr2->ambientColor = ambientColor;
	//dataPtr2->diffuseColor = diffuseColor;
	//dataPtr2->lightDirection = lightDirection;

	//// Unlock the constant buffer.
	//deviceContext->Unmap(m_lightBuffer, 0);

	//// Set the position of the light constant buffer in the pixel shader.
	//bufferNumber = 0;

	//// Finally set the light constant buffer in the pixel shader with the updated values.
	//deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	// Set the vertex input layout.

	context.GetDeviceContext()->IASetInputLayout(objectToRender.vertLayout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
	context.GetDeviceContext()->VSSetShader(objectToRender.vertexShader, NULL, 0);
    context.GetDeviceContext()->PSSetShader(objectToRender.pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	context.GetDeviceContext()->PSSetSamplers(0, 1, &objectToRender.sampleState);

	// Render the triangle.
	context.GetDeviceContext()->DrawIndexed(objectToRender.indexCount, 0, 0);	
}