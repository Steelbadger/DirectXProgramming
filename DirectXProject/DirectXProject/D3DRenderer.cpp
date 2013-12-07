#include "D3DRenderer.h"
#include <iostream>
#include <D3DX11tex.h>
#include <D3Dcompiler.h>
#include "GraphicsWindow.h"

struct SimpleVertex
{
	D3DXVECTOR3 Pos;
    D3DXVECTOR2 Tex;
};

struct CBChangesEveryFrame
{
	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProjection;
    D3DXVECTOR4 vMeshColor;
};

D3DRenderer::D3DRenderer(void)
{
}


D3DRenderer::~D3DRenderer(void)
{
}

void D3DRenderer::Initialize(Window& win)
{
	context.Initialize(win);
	window = &win;

	HRESULT hr;
    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( "Tutorial07.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
    }

    // Create the vertex shader
	hr = context.GetDevice()->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
    if( FAILED( hr ) )
    {    
        pVSBlob->Release();
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
    hr = context.GetDevice()->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
    pVSBlob->Release();

    // Set the input layout
    context.GetDeviceContext()->IASetInputLayout( g_pVertexLayout );

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( "Tutorial07.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
    }

    // Create the pixel shader
    hr = context.GetDevice()->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
    pPSBlob->Release();

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },

        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },

        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },

        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },

        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },

        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = context.GetDevice()->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );


    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    context.GetDeviceContext()->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Create index buffer
    // Create vertex buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = context.GetDevice()->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );


    // Set index buffer
    context.GetDeviceContext()->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
    context.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    // Create the constant buffers
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr =  context.GetDevice()->CreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );


    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile( context.GetDevice(), "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );


    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = context.GetDevice()->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
 
    // Initialize the world matrices
    D3DXMatrixIdentity(&g_World);

    // Initialize the view matrix
    D3DXVECTOR3 Eye = D3DXVECTOR3( 0.0f, 3.0f, -6.0f);
    D3DXVECTOR3 At = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 Up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up );
}

void D3DRenderer::Load(World& w)
{
	world = &w;
}

void D3DRenderer::Render()
{
    // Update our time
    static float t = 0.0f;

    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if( dwTimeStart == 0 )
        dwTimeStart = dwTimeCur;
    t = ( dwTimeCur - dwTimeStart ) / 1000.0f;


    // Rotate cube around the origin
	D3DXMatrixRotationY(&g_World, t );


    //
    // Update variables that change once per frame
    //
    CBChangesEveryFrame cb;
    D3DXMatrixTranspose(&cb.mWorld, &g_World);
    cb.vMeshColor.x = ( sinf( t * 1.0f ) + 1.0f ) * 0.5f;
    cb.vMeshColor.y = ( cosf( t * 3.0f ) + 1.0f ) * 0.5f;
    cb.vMeshColor.z = ( sinf( t * 5.0f ) + 1.0f ) * 0.5f;


    D3DXMatrixTranspose(&cb.mView, &g_View);
    // Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(&g_Projection, 1.6, window->GetWidth() / window->GetHeight(), 0.01f, 100.0f );
    
    D3DXMatrixTranspose(&cb.mProjection, &g_Projection);

	context.GetDeviceContext()->UpdateSubresource( g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0 );

    //
    // Render the cube
    //
    context.GetDeviceContext()->VSSetShader( g_pVertexShader, NULL, 0 );
    context.GetDeviceContext()->VSSetConstantBuffers( 0, 1, &g_pCBChangesEveryFrame );
    context.GetDeviceContext()->PSSetShader( g_pPixelShader, NULL, 0 );
    context.GetDeviceContext()->PSSetConstantBuffers( 0, 1, &g_pCBChangesEveryFrame );
    context.GetDeviceContext()->PSSetShaderResources( 0, 1, &g_pTextureRV );
    context.GetDeviceContext()->PSSetSamplers( 0, 1, &g_pSamplerLinear );
    context.GetDeviceContext()->DrawIndexed( 36, 0, 0 );

}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT D3DRenderer::CompileShaderFromFile( CHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )		//  Helper function for compiling any type of shader?
{
    HRESULT hr = S_OK;
	LPCSTR thingy = szFileName;
    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( thingy, NULL, NULL, szEntryPoint, szShaderModel, 
        D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

